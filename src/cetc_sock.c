/*
 * cetc_sock.c
 *
 *  Created on: Jan 13, 2017
 *      Author: root
 */
#include <time.h>
#include "cetc_sock.h"
int sockfd = 0 ;
struct sockaddr_in servaddr ={0,0};

int flag = 0 ;
int i_heart_num = 0;//
int init_flag = 0 ;
// 短整型大小端互换
typedef unsigned short int uint16;
typedef unsigned long int uint32;
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
							(((uint16)(A) & 0x00ff) << 8))

// 长整型大小端互换

#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                           (((uint32)(A) & 0x00ff0000) >> 8) | \
                           (((uint32)(A) & 0x0000ff00) << 8) | \
                           (((uint32)(A) & 0x000000ff) << 24))
int checkCPUendian()
{
      union{
             unsigned long int i;
             unsigned char s[4];
      }c;

      c.i = 0x12345678;
      return (0x12 == c.s[0]);
}
unsigned short int t_htons(unsigned short int h)
{
      // 若本机为大端，与网络字节序同，直接返回
      // 若本机为小端，转换成大端再返回
      return checkCPUendian() ? h : BigLittleSwap16(h);
}



int udp_client_init(char *ip,int port) {
	bzero(&servaddr,sizeof(servaddr)) ;
	servaddr.sin_family = AF_INET ;
	servaddr.sin_port = t_htons(port)  ;
	servaddr.sin_addr.s_addr = inet_addr(ip) ;
	return 1 ;
}
int udp_init(){
	struct sockaddr_in addrsrv ;
	sockfd = socket(PF_INET,SOCK_DGRAM,0) ;
	if(sockfd<0)
		return -1 ;
	bzero(&addrsrv,sizeof(addrsrv)) ;
	addrsrv.sin_family = AF_INET ;
	addrsrv.sin_port = htons(44567) ;
	addrsrv.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(ip) ;
	int nRet = 0 ;
	nRet = bind(sockfd,(struct sockaddr*)&addrsrv,sizeof(addrsrv));
	if(nRet == 0)
		return 1 ;
	else
		return -1 ;
}
int udp_close() {
	if(sockfd<0)
		return -1 ;
	close(sockfd);
	return 1 ;
}
int udp_sockopt(int t){
	if(t>0){
		if(sockfd>0){
			struct timeval tv_out ;
			tv_out.tv_sec = t ;
			tv_out.tv_usec = 0 ;
			int ret = setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv_out,sizeof(tv_out));
			if(ret == 0){
				return 1 ;
			}
		}
	}
	return -1 ;
}
int udp_send(char *buf,int len) {
	ssize_t Num = sendto(sockfd,buf,len,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(Num == -1)
		return -1 ;
	return 1 ;
}
int udp_recv(char *buf,int *len) {
	int s_len = sizeof(servaddr);
	ssize_t num =recvfrom(sockfd,buf,*len,0,(struct sockaddr*)&servaddr,&s_len);
	*len = 0 ;
	if(num == -1)
		return -1 ;
	*len = num ;
	return 1 ;
}
char sock_send_get_buf[1800];
int sock_send_get_length ;
void* thread_sock_recv(void *arg){
	usleep(1000*10);
	if(flag == 0){
		udp_init();
		udp_sockopt(5) ;
		flag = 1 ;
	}
	while(flag == 0){
		usleep(100);
	}
	udp_client_init(pt_ip,pt_port) ;
//	printf("%s %d\n",pt_ip,pt_port) ;
	int len = 0 ;
	int ret = 0 ;
	char buf[1500]={0};
	while(1){
		usleep(1000);
		len = 1500 ;
		ret = udp_recv(buf,&len);
		if(ret == -1){
			continue ;
		}else if(len>0){
			if(!memcmp(buf,udp_head_set_xyz,8)){
				udp_set_xyz(buf,len,udp_data_to_fy,&udp_id,&udp_num) ;
			}else if(!memcmp(buf,udp_head_get_order,8)){
				udp_get_order(buf,len,&udp_id,&udp_order) ;
			}
		}
		usleep(1000*20) ;
	}
	return NULL ;
}
//void cetc_create_thread() {
//    pthread_t th_recv ;
//    int ret;
//    int *thread_ret = NULL;
//    ret = pthread_create(&th_recv,NULL,thread_sock_recv,NULL) ;
//  //  printf( "This is the main process.\n" );
//  //  pthread_join(th_recv,NULL);
//  //  printf( "thread_ret = %d.\n", *thread_ret );
//}

