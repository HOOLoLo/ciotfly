/*
 * server_sock.c
 *
 *  Created on: Nov 3, 2017
 *      Author: root
 */
#include "log.h"
#include "server_sock.h"
#include "ancom.h"
struct sockaddr_in serveraddr = {0} ;
char pix_buf[30] = {0} ;
int pix_flag = 0 ;
int udp_server_client_init(char *ip,int port) {
	bzero(&serveraddr,sizeof(serveraddr)) ;
	serveraddr.sin_family = AF_INET ;
	serveraddr.sin_port = t_htons(port)  ;
	serveraddr.sin_addr.s_addr = inet_addr(ip) ;
	return 1 ;
}
int udp_server_init(){
	struct sockaddr_in addrsrv ;
	sock_serverfd = socket(PF_INET,SOCK_DGRAM,0) ;
	if(sock_serverfd<0)
		return -1 ;
	int port = 0 ;
	port = 23456;
	bzero(&addrsrv,sizeof(addrsrv)) ;
	addrsrv.sin_family = AF_INET ;
//	addrsrv.sin_port = htons(44567) ;
	addrsrv.sin_port = htons(port) ;
	addrsrv.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(ip) ;
	int nRet = 0 ;
	nRet = bind(sock_serverfd,(struct sockaddr*)&addrsrv,sizeof(addrsrv));
	if(nRet == 0)
		return 1 ;
	else
		return -1 ;
}
int udp_server_close() {
	if(sock_serverfd<0)
		return -1 ;
	close(sock_serverfd);
	return 1 ;
}
int udp_server_sockopt(int t){
	if(t>0){
		if(sock_serverfd>0){
			struct timeval tv_out ;
			tv_out.tv_sec = t ;
			tv_out.tv_usec = 0 ;
			int ret = setsockopt(sock_serverfd,SOL_SOCKET,SO_RCVTIMEO,&tv_out,sizeof(tv_out));
			if(ret == 0){
				return 1 ;
			}
		}
	}
	return -1 ;
}
int udp_server_send(char *buf,int len) {
	ssize_t Num = sendto(sock_serverfd,buf,len,0,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(Num == -1)
		return -1 ;
	return 1 ;
}
int udp_server_recv(char *buf,int *len) {
	int s_len = sizeof(serveraddr);
	ssize_t num =recvfrom(sock_serverfd,buf,*len,0,(struct sockaddr*)&serveraddr,&s_len);
	if(num == -1){
		*len = 0 ;
		return -1 ;
	}
	*len = num ;
	return 1 ;
}
void* thread_server_sock_recv(void *arg){
	usleep(1000*10);
	udp_server_init();
	udp_server_sockopt(5) ;
	int len = 0 ;
	int ret = 0 ;
	short i = 0 ;
	char buf[1500]={0};
	char sed[20]={0x01} ;
	int udp__index = 0 ;
	short x = 0 ;
	short y = 0 ;
	while(1){
		len = 150 ;
		memset(buf,'\0',len) ;
		ret = udp_server_recv(buf,&len);
		if(ret==1){
			udp__index++ ;
			if(udp__index%10==0)
			printf("------udp num %d\n",udp__index) ;
			//if(udp__index%2==0)
			//	continue ;
			if(len>10&&len<=17){
				i = *((short*)(buf+11)) ;
				if(i>5&&i<175){
					//pthread_mutex_lock(&mutex);
					//com_send(buf,len) ;
					//pthread_mutex_unlock(&mutex);
					memcpy(pix_buf,buf,len) ;
					pix_flag = 3 ;
				}
			}else if(len > 19){
				i = *((short*)(buf+11)) ;
				if(i>5&&i<175){
					//pthread_mutex_lock(&mutex);
					//com_send(buf,17) ;
					//pthread_mutex_unlock(&mutex);
					pthread_mutex_lock(&mutex_log) ;
					memcpy(pix_buf,buf,17) ;
					pix_flag = 3 ;
					pthread_mutex_unlock(&mutex_log) ;
				}
				x  = *((short*)(buf+17)) ;
				y  = *((short*)(buf+19)) ;

				pthread_mutex_lock(&mutex_log) ;

				pix_x = x;
				pix_y = y ;

				if(x!=0&&y!=0){
					if(udp_send_flag==0){
						//printf("pix two------------\n") ;
						char sed[2]={0,0} ;
						if(point_short[current_point][4]==1){
							sed[0]=0x01 ;//blue
						}else if(point_short[current_point][4]==2){
							sed[0]=0x02 ;//red
						}
						if(udp__index%10==0){
						//	printf("pix two------------%d, %d,%d\n",current_point,sed[0]&0xff,buf[len-1]&0xff) ;
							udp_server_send(sed,1) ;
						}
					}
				}
				pthread_mutex_unlock(&mutex_log) ;
				//pthread_mutex_lock(&mutex_log) ;
				//MY_LOG("pix x:%d,y:%d\n",x,y) ;
				//pthread_mutex_unlock(&mutex_log) ;
				if(udp__index%10==0)
				printf("------------------------pix %d,%d\n",x,y) ;
			}
		}
		usleep(1000*50) ;
	}
	return NULL ;
}
void* thread_server_sock_com(void *arg) {
	char buf[30] ;
    int pix_mid_x = 992 ;
    int pix_mid_y = 2349 ;
    char send_buf[100] ;
    int send_len ;
    int x = 0 ;
    int y = 0 ;
    int z = 0 ;
	while(1){
		if(pix_flag==3){
			pthread_mutex_lock(&mutex_log) ;
			memcpy(buf,pix_buf,17) ;
			pix_flag = 0 ;
			pthread_mutex_unlock(&mutex_log) ;
//			com_send(buf,17) ;
		}
		if(udp_send_flag == 3){
			if(pix_x!=0&&pix_y!=0){
			//	y = pix_mid_y+((pix_x-320)*10)/15 ;
			//	x = pix_mid_x+((pix_y-240)*10)/15 ;
				if(current_point==0)
					continue ;
				if(point_short[current_point][4]==1){
					y = point_short[current_point][5]+((pix_x-320)*10)/15 ;
					x = point_short[current_point-1][5]+((pix_y-240)*10)/15 ;
				}else if(point_short[current_point][4]==2){
					y = point_short[current_point-1][5]+((pix_x-320)*10)/15 ;
					x = point_short[current_point][5]+((pix_y-240)*10)/15 ;
				}
			printf("pix x=%d,y=%d\n",x,y) ;
				set_posxyz(x,y,z,send_buf,&send_len) ;
//				com_send(send_buf,send_len) ;
			}
		}
		usleep(1000*100) ;
	}
}
void server_create_thread(){
    pthread_t th_recv ;
    int ret;
    int *thread_ret = NULL;
    ret = pthread_create(&th_recv,NULL,thread_server_sock_recv,NULL) ;
    ret = pthread_create(&th_recv,NULL,thread_server_sock_com,NULL) ;
}
