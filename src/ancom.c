/*
 * ancom.c
 *
 *  Created on: Oct 18, 2017
 *      Author: root
 */
#include <sys/time.h>
#include "ancom.h"
#include "app_user.h"
#include "cetc_sock.h"
#include "server_sock.h"
int com_send(char *buf,int len) {
	pthread_mutex_lock(&mutex);
	serialWrite(1, 38400,buf,len);
	pthread_mutex_unlock(&mutex);
	return 1 ;
}
int com_recv(char *buf,int len){
//	pthread_mutex_lock(&mutex);
	serialRead(1,38400,buf,len) ;
//	pthread_mutex_unlock(&mutex);
	return 1 ;
}

void* thread_com_recv(void *arg){
	char buf[100] ={0};
	char send[100] ={0};
	int len=0 ;
	int x=0,y=0,z=0 ;
	int flag = 0 ;
	while(1){
		usleep(1000*10) ;
		len = 25 ;
		com_recv(buf,len) ;
		usleep(1000*10) ;
		get_posxyz(buf,len,&x,&y,&z,&flag) ;
		if(flag == 1){
			udp_xyz(x,y,z,send,&len) ;
			udp_send(send,len) ;
		}
	}
	return NULL ;
}
void* thread_com_send(void *arg){
	struct timeval tvafter,tvpre;
	struct timezone tz;
	printf("start\n") ;
	while(1){
		usleep(1) ;
		if(send_flag == 3){
			//printf("send data\n") ;
			gettimeofday (&tvpre , &tz);
			//pthread_mutex_lock(&mutex_send) ;
			m_send_len = send_len ;
			memcpy(m_send_buf,send_buf,m_send_len) ;
			send_flag = 0 ;
			//pthread_mutex_unlock(&mutex_send) ;
			//com_send(m_send_buf,m_send_len) ;
			serialWrite(1, 38400,m_send_buf,m_send_len);
			//printf("send\n") ;
			gettimeofday (&tvafter , &tz);
			printf("send 花费时间:%d %d\n",(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000,m_send_len);
		}
	}
	return NULL ;
}
void cetc_create_com_thread(){
    pthread_t th_com ;
    int ret;
    int *thread_ret = NULL;
    ret = pthread_create(&th_com,NULL,thread_com_recv,NULL) ;
    ret = pthread_create(&th_com,NULL,thread_com_send,NULL) ;
}
