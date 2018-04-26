/*
 * iotfly.c
 *
 *  Created on: Oct 12, 2017
 *      Author: root
 *      raspberrypi
 *      pi@192.168.1.151
 */

#include     <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stddef.h>
#include <sys/time.h>
#include "uart.h"
#include "analy.h"
#include "ancom.h"
#include "signal.h"
#include "cetc_sock.h"
#include "server_sock.h"
#include "point_s.h"
#include "log.h"
#include "threadi.h"
#include "client_tcp.h"
#include "unidimensional.h"
int sock_fd = 0 ;
int connect_fdd = 0 ;
int sock_fd_flag = 0 ;
//
struct _POINT_ gpoint[20]={{0,0,0}} ;
int point_num_all=0 ;
int point_num_current=0 ;
int one_point_num = 0 ;
float inputData[20][4]={{0,0,0,0}} ;
int  one_inputData[20][4]={{0,0,0,0}} ;
short last_point[10][3]={{0,0,0}} ;
short avg_point[3] = {0,0,0} ;
float output[5]={0.0,0.0,0.0} ;
int one_output[3]={0,0,0,0} ;
short point_short[20][6]={{0,0}} ;
short ab_point[20][2]={{0,0}} ;
short point_num = 0 ;
short current_point = 0 ;
int direction_i = 1 ;//1 pre,2,back
int dimension_i = 0 ;//1,一维,2,二维
int process_start = 0 ;//0 stop,1 start

int last_num = 0 ;
short i_lr = 0 ;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_serial = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_send = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER ;//
char send_buf[100]={0} ;
int send_len=0 ;
int send_flag = 0 ;
int front_back = 0 /*0,1,x+,2,x-,3,y+,4,y-*/ ;
int judge_dir=0 ;
int avg_d[10] = {0} ;
int avg_index = 0 ;
int width_length = 0 ;


int pix_x = 0 ;
int pix_y = 0 ;


int udp_send_flag = 0 ;


int main(void){
	char *com = "/dev/ttyUSB0" ;
	sleep(1) ;
	char mac[20] ;
	read_file_info();
	read_points(1) ;//2017-11-15
	read_ip_port() ;

	front_back = 0 ;//这个变量是干嘛的

	sleep(1) ;

	pthread_mutex_init(&mutex,NULL);

	pthread_mutex_init(&mutex_send,NULL);
	pthread_mutex_init(&mutex_serial,NULL);
	pthread_mutex_init(&mutex_log,NULL);

	cetc_create_send_thread();

	server_create_thread();

	init_position() ;
	//2017-11-25
	tcp_create_thread() ;

	usleep(1000*15);
	while (1) { //循环读取数据
		if(send_flag == 3){
			if(send_len>100)
				send_len = 0 ;
			m_send_len = send_len ;
			memcpy(m_send_buf,send_buf,m_send_len) ;
			send_flag = 0 ;
			//printf("send x,y,z 1\n") ;
			//向无人机发送串口数据，还是用的旧板子的函数，这里先注释掉
			com_send(m_send_buf,m_send_len) ;

			//printf("send x,y,z 2\n") ;
//			gettimeofday (&tvafter , &tz);
//			printf("send 花费时间:%d %d\n",(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000,m_send_len);
		}else{
			usleep(1000) ;
		}
	}
	close_position() ;
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_send);
	pthread_mutex_destroy(&mutex_serial) ;
	pthread_mutex_destroy(&mutex_log) ;
	return EXIT_SUCCESS;
}
