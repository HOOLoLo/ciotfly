/*
 * threadi.c
 *
 *  Created on: Nov 17, 2017
 *      Author: root
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "threadi.h"
#include "uart.h"
#include "analy.h"
#include "ancom.h"
#include "signal.h"
#include "cetc_sock.h"
#include "server_sock.h"
#include "point_s.h"
#include "log.h"
#include "client_tcp.h"
#include "serial_usb_com.h"
void* thread_buf_recv(void *arg){
	char buf[100] ;
	int len=100 ;
	int x=0,y=0,z=0 ;
	while(1){
		len = 100 ;
		memset(buf,'\0',100) ;
		//pthread_mutex_lock(&mutex);
//		com_recv(buf,len) ;
		//pthread_mutex_unlock(&mutex);
//		get_contr1_xyz(buf,30,&x,&y,&z) ;
		usleep(1000*180) ;
		if(x*y*z>10){
			printf("%d,%d,%d\n",x,y,z) ;
		}
	}
	return NULL ;
}
int  judge_position_i = 0 ;
int  judge_position_ii = 0 ;
int  judge_position_sum = 0 ;

char send_point[50] ;
int  send_point_len ;

void judge_next_point(char *com,int para) {
//	printf("judge_position_ii %d\n",judge_position_sum) ;
	if(judge_position_ii==0||judge_position_ii==-1){
		judge_position_sum = 0;
		judge_position_ii = judge_position_i ;
	}else if(judge_position_ii == 1){
		judge_position_sum++;
		judge_position_ii = judge_position_i ;
		if(judge_position_sum>3){
			if(direction_i==1){
				if(current_point <point_num-1){
					current_point++ ;
					dimension_i = point_short[current_point][3] ;
					//serial_set_group(point_short[current_point][2]) ;
					judge_position_sum=0;
					char sed[2]={3,0} ;
					udp_server_send(sed,1) ;
					udp_send_flag = 3 ;
					serial_set_group(point_short[current_point][2]) ;
					if(point_short[current_point][4]==1){
						sed[0]=0x01 ;//blue
					}else if(point_short[current_point][4]==2){
						sed[0]=0x02 ;//red
					}
					usleep(1000*900) ;
					udp_server_send(sed,1) ;
					udp_send_flag = 0 ;
					point_num_current = 0 ;
				}else if(current_point == point_num-1){
					process_start = 0;//0 stop,1 start
					//serial_set_group(point_short[current_point][2]) ;
					judge_position_sum = 0;
					char sed[2]={3,0} ;
					udp_send_flag = 3 ;
					udp_server_send(sed,1) ;
					serial_set_group(point_short[current_point][2]) ;
					if(point_short[current_point][4]==1){
						sed[0]=0x01 ;//blue
					}else if(point_short[current_point][4]==2){
						sed[0]=0x02 ;//red
					}
					usleep(1000*900) ;
					udp_server_send(sed,1) ;
					udp_send_flag = 0 ;
				}
			}/*else if(direction_i == 2){
				if(current_point<point_num-1){
					current_point++ ;
					dimension_i = point_short[point_num-current_point-2][2] ;

					serial_set_group(point_short[point_num-current_point-2][2]) ;
					judge_position_sum = 0;

				}else if(current_point == point_num -1){
					process_start = 0;//0 stop,1 start
					serial_set_group(point_short[point_num-current_point-2][2]) ;
					judge_position_sum = 0;

				}
			}*/
		}
	}
}
//
void* thread_data_recv(void *arg){
	char *com = "/dev/ttyUSB0" ;
	int len;
	int out=0 ;
	int ibx ;
	int icy ;
	int ret ;
	short x=0,y=0,z=0 ;
	char rcv_buf[200] ;
	char sed_buf[100] ;
	char p_buf[100] ;
	int p_len = 0 ;
	char mac[80] ;
	int out_num = 0 ;
	int  para=921600 ;
	while(open_serial(com)==-1){
		sleep(10) ;
	}
	sleep(1);
	int avg_one[5] ;
	int avg_one_index = 0 ;
	int avg_one_num = 0 ;

	usleep(1000*10) ;
	direction_i = 1 ;
    current_point = 0 ;
    //serial_set_group(0x01) ;
    while(point_short[current_point][2]!=0){
    	serial_set_group(point_short[current_point][2]) ;
		char sed[2]={3,0} ;
		if(point_short[current_point][4]==1){
			sed[0]=0x01 ;//blue
		}else if(point_short[current_point][4]==2){
			sed[0]=0x02 ;//red
		}
		udp_server_send(sed,1) ;
    	usleep(10) ;
    	break ;
    }
 //   char cmp[20]="32552428" ;
    int pix_mid_x = 992 ;
    int pix_mid_y = 2349 ;
	while (1) { //循环读取数据
//	  printf("recv data --------------100\n") ;
	  len = 100 ;
	  usleep(10) ;
	  memset(rcv_buf,'\0',len) ;
	  serial_read(rcv_buf,&len) ;
	  rcv_buf[len] = '\0';
//	  printf("%s",rcv_buf) ;
	  if(len > 0&&len<80){
		memset(mac,'\0',50) ;
		ret = get_info(rcv_buf,len,mac,&ibx,&icy) ;
//		printf("%s,%d\n",mac,ibx) ;
		if(ret == -1){
			continue ;
		}
//		MY_LOG("%s,%d\n",mac,ibx) ;
		//取当前点的配置信息，确定算法，一维、二维
		dimension_i = point_short[current_point][3] ;

		if(dimension_i ==2){
		//二维
			out = 0 ;
			set_info(ret,mac,(float)((ibx+1)/100.0),&out) ;
			if(out==0xff)
			{
				x=(short)(output[0]*100) ;
				y=(short)(output[1]*100) ;
				z=(short)(output[2]/20) ;
				//printf("%d,%d,%d\n",x,y,z) ;
				pthread_mutex_lock(&mutex_log) ;

				if(pix_x!=0){
				//	y = pix_mid_y+((pix_x-320)*10)/15 ;
					y = point_short[current_point][5]+((pix_x-320)*10)/15 ;
					//pix_x = 0 ;
				}
				if(pix_y!=0){
				//	x = pix_mid_x+((pix_y-240)*10)/15 ;
					x = point_short[current_point][5]+((pix_y-240)*10)/15 ;
					//pix_y = 0 ;
				}
				pthread_mutex_unlock(&mutex_log) ;
				memset(sed_buf,'\0',100) ;
				//printf("%d,%d,%d\n",x,y,z) ;
				if((x>=0)&&(y>=0)&&(x<maxX)&&(y<maxY))
				{
					set_posxyz(x,y,z,sed_buf,&len) ;
					if(len>100)
						continue ;
					memcpy(send_buf,sed_buf,len) ;
					send_len = len ;
					send_flag = 3 ;
					printf("x:%d,y:%d,z:%d---------------\n",x,y,z) ;
					//pthread_mutex_unlock(&mutex_log) ;
					/*avg_one_index++ ;
					if(avg_one_index%10==0){
						if(sock_fd_flag!=0){
							tcp_xyz(x,y,z,p_buf,&p_len) ;
							tcp_send(p_buf,p_len) ;
						}
					}*/
					//if(process_start == 1)
					{
						judge_position_i = judge_position(x,y,direction_i,current_point,10) ;
						judge_next_point(com,para) ;
					}
				}else{
					usleep(1) ;
				}
			}
		}else if(dimension_i == 1){
			out = 0 ;
			//set_one_info(ret,mac,ibx,&out,55) ;
//			printf("%s %d\n",mac,ibx) ;
			set_one_dimension(ret,mac,(float)((ibx+1)/100.0),&out) ;
			if(out==0xff){
				x=(short)(output[0]*100) ;
				y=(short)(output[1]*100) ;
				z=(short)(output[2]/20) ;
				//printf("xyz,%d,%d,%d\n",x,y,z) ;
				pthread_mutex_lock(&mutex_log) ;
				if(pix_x!=0){
				//	y = pix_mid_y+((pix_x-320)*10)/15 ;
					y = point_short[current_point][5]+((pix_x-320)*10)/15 ;
					//pix_x = 0 ;
				}
				if(pix_y!=0){
				//	x = pix_mid_x+((pix_y-240)*10)/15 ;
					x = point_short[current_point][5]+((pix_y-240)*10)/15 ;
					//pix_y = 0 ;
				}
				pthread_mutex_unlock(&mutex_log) ;
				memset(sed_buf,'\0',100) ;
				if((x>=0)&&(y>=0)&&(x<maxX)&&(y<maxY))
				{
					set_posxyz(x,y,z,sed_buf,&len) ;
					memcpy(send_buf,sed_buf,len) ;
					send_len = len ;
					send_flag = 3 ;

					/*avg_one_index++ ;
					if(avg_one_index%10==0){
						if(sock_fd_flag!=0){
							tcp_xyz(x,y,z,p_buf,&p_len) ;
							ret = tcp_send(p_buf,p_len) ;
						}
					}*/
					printf("x:%d,y:%d,z:%d\n",x,y,z) ;
					//if(process_start == 1)
					{
						judge_position_i = judge_position(x,y,direction_i,current_point,20) ;
						judge_next_point(com,para) ;
					}
				}
			}
		}

	   }else{
		   usleep(1) ;
	   }
		usleep(1);
	}
	return NULL ;
}

void cetc_create_send_thread() {
    pthread_t th_com ;
    int ret;
    int *thread_ret = NULL;
    ret = pthread_create(&th_com,NULL,thread_data_recv,NULL) ;
    ret = pthread_create(&th_com,NULL,thread_buf_recv,NULL) ;
}
