/*
 * client_tcp.c
 *
 *  Created on: Nov 19, 2017
 *      Author: root
 */
#include "analy.h"
#include "client_tcp.h"
#include "signal.h"
#include "point_s.h"
#include "server_sock.h"
#define MAXNUM 1600

/*
// 短整型大小端互换
typedef unsigned short int uint16;
typedef unsigned long int uint32;
#define tcpBigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
							(((uint16)(A) & 0x00ff) << 8))

// 长整型大小端互换

#define tcptcpBigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                           (((uint32)(A) & 0x00ff0000) >> 8) | \
                           (((uint32)(A) & 0x0000ff00) << 8) | \
                           (((uint32)(A) & 0x000000ff) << 24))
int tcpcheckCPUendian()
{
      union{
             unsigned long int i;
             unsigned char s[4];
      }c;

      c.i = 0x12345678;
      return (0x12 == c.s[0]);
}
unsigned short int tcpt_htons(unsigned short int h)
{
      // 若本机为大端，与网络字节序同，直接返回
      // 若本机为小端，转换成大端再返回
      return tcpcheckCPUendian() ? h : tcpBigLittleSwap16(h);
}*/

int tcp_client_init(char *ip,int port) {
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd <0)
		return -1 ;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port =t_htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	socklen_t addr_len = sizeof(server_addr);
    int connect_fd = connect(sock_fd, (struct sockaddr*)&server_addr, addr_len);
//    fcntl(sock_fd,F_SETFL,O_NONBLOCK);
//   tcp_sockopt(5) ;
//    printf("connect %d\n",connect_fd) ;
    connect_fdd = connect_fd ;
    if(connect_fd < 0){
    	sock_fd=-1 ;
//        perror("connect");
          return -1;
    }
//    printf("connect_fd %d\n",connect_fd) ;
	return 1;
}
int tcp_init() {
	g_index_num = 0 ;
	g_index_send_num = 0 ;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd <0)
		return -1 ;
	return 1;
}
int tcp_close() {
	sock_fd_flag = 0  ;
	close(sock_fd) ;
	sock_fd = 0 ;
	return 1;
}
int tcp_sockopt(int t){
	if(t>0){
		if(sock_fd>0){
			struct timeval tv_out ;
			tv_out.tv_sec = t ;
			tv_out.tv_usec = 0 ;
			int ret = setsockopt(sock_fd,SOL_SOCKET,SO_RCVTIMEO,&tv_out,sizeof(tv_out));
			if(ret == 0){
				return 1 ;
			}
		}
	}
	return -1 ;
}
int tcp_send(char *buf,int len) {
	int ret=0 ;
//	printf("tcp_send %d,%d,%d\n",sock_fd,connect_fdd,len) ;
	if((sock_fd>0)&&(connect_fdd>=0)){
		ret = write(sock_fd,buf,len);
//		printf("write sock ---------- %d,%d\n",sock_fd,len) ;
		if(ret == -1){
			tcp_close();
			return -1 ;
			if(errno == EINTR ||errno == EWOULDBLOCK){
				return 2 ;
			}else{
				tcp_close();
				return -1 ;
			}
		}
	}else{

		return -2 ;
	}
	return ret;
}
int tcp_recv(char *buf,int *len) {
	int ret = 0 ;
	if(sock_fd>0){
		ret = read(sock_fd,buf,*len);
		if(ret < 0){
			if(errno == EINTR || errno == EWOULDBLOCK){
				return 2 ;
			}else{
				tcp_close() ;
				return -1;
			}
		}else if(ret == 0){
			tcp_close() ;
			return -1 ;
		}
	}
	return ret;
}
void* thread_socktcp_init(void *arg){
	int ret = 0;
	while(1){
		if(sock_fd_flag == 0){
			if((ret = tcp_client_init(pt_ip,pt_port))==1)
				sock_fd_flag = 1 ;
		}else {
			sleep(30) ;
		}
		sleep(10) ;
	}
	return NULL ;
}
void* thread_socktcp_send(void *arg){
	while(1){
		if(sock_fd_flag !=0){

		}else{
			sleep(1) ;
		}
	}
	return NULL ;
}
void printf_str(char *buf,int len) {
	int i=0 ;
	for(i=0 ;i<len;i++){
		printf(" %02x ",buf[i]&0xff) ;
	}
	printf("\n") ;
}/*
 * 10 :仓库停机坪 ---->4号停机坪 0x0a
 * 11 :仓库停机坪 ---->5号停机坪 0x0b
 * 12 ：4号停机坪 ---->仓库停机坪0x0c
 * 13 ：5号停机坪 ---->仓库停机坪0x0d
*/
void* thread_socktcp_recv(void *arg){
	int ret ;
	char buf[MAXNUM] ;
	int len ;
	int order_num_i = 0 ;
	while(1){
		if(sock_fd_flag!=0){
			len = MAXNUM ;
			memset(buf,'\0',MAXNUM) ;
			ret = tcp_recv(buf,&len);
			while(ret>8){
				if(!memcmp(buf,tcp_head_order,2)){
					//char *com="/dev/ttyUSB0" ;
					//int para = 921600 ;
					switch(buf[3]&0xff){
					case 0x0f:
						ret = 0 ;
						break ;
					case 0x02:
					//	printf_str(buf,ret) ;
						tcp_num(buf,&order_num) ;
					//	read_points(order_num) ;
						current_point = 0 ;
						direction_i = 0 ;
						ret -= 12 ;
						if(ret>5)
						memcpy(buf,buf+12,ret) ;
						break ;
					case 0x03:
					//	printf_str(buf,ret) ;
						current_point = 0 ;
						tcp_fun_to(buf,&order_num,&tcp_to) ;
						read_points(order_num) ;
						serial_set_group(point_short[0][2]) ;
						direction_i = 1 ;
						current_point = 0 ;
						dimension_i = point_short[0][3] ;
						char lin ;
						if(order_num == 1)
							lin=0x0a ;
						else if(order_num == 2)
							lin = 0x0b ;
						memset(buf,'\0',MAXNUM) ;
						set_contr6_line(lin,buf,&len) ;
						//pthread_mutex_lock(&mutex);
//						com_send(buf,len) ;
						//pthread_mutex_unlock(&mutex);

						process_start = 1;//0 stop,1 start
						sleep(5) ;
						memset(buf,'\0',MAXNUM) ;
						len = 0 ;
						set_contr1_start(30,10,buf,&len) ;
						ret -= 13 ;
						if(ret>5)
						memcpy(buf,buf+13,ret) ;
						break ;
					case 0x04 :
					//	printf_str(buf,ret) ;
						current_point = 0 ;
						tcp_fun_back(buf,&order_num,&tcp_back) ;
						read_points(order_num+2) ;
						if(order_num == 1)
							lin=0x0c ;
						else if(order_num == 2)
							lin = 0x0d ;
						memset(buf,'\0',MAXNUM) ;
						set_contr6_line(lin,buf,&len) ;

//						com_send(buf,len) ;
						//serial_set_group(point_short[point_num-2][2]) ;
						serial_set_group(point_short[0][2]) ;
						direction_i = 1 ;
						current_point = 0 ;
						//dimension_i = point_short[point_num-2][3] ;
						dimension_i = point_short[0][3] ;
						process_start = 1;//0 stop,1 start
						sleep(5) ;
						memset(buf,'\0',MAXNUM) ;
						len = 0 ;
						set_contr1_start(30,10,buf,&len) ;
						ret -= 13 ;
						if(ret>5)
						memcpy(buf,buf+13,ret) ;
						break ;
					default:
						ret = 0 ;
						break ;
					}
				}
			}
		}else{
			sleep(10) ;
		}
		usleep(100) ;
	}
	return NULL ;
}
void tcp_create_thread() {
    pthread_t th_recv ;
    int ret = 0;
    ret = pthread_create(&th_recv,NULL,thread_socktcp_init,NULL) ;
    ret = pthread_create(&th_recv,NULL,thread_socktcp_recv,NULL) ;
}
