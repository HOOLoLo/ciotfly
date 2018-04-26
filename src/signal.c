/*
 * signal.c
 *
 *  Created on: Oct 20, 2017
 *      Author: root
 */
#include "signal.h"
#include "server_sock.h"

char head_posxyz[8]={0x4D,0x4B,0x01,0x06,0x01,0x0A,0x00} ;
char head_order[8]={0x4D,0x4B,0x01,0x03,0x01,0x21,0x00} ;
char head_getxyz[8]={0x4D,0x4B,0x01,0x02,0x03,0x10,0x00}  ;
char head_line[8] = {0x4D,0x4B,0x01,0x03,0x02,0x06,0x00} ;
char udp_head_xyz[8]={0x4D,0x4B,0x01,0x01,0x01,0x00,0x00,0x00} ;
char udp_head_order[8]={0x4D,0x4B,0x01,0x0F,0x01,0x00,0x00,0x00} ;
char udp_head_set_xyz[8]={0x4D,0x4B,0x01,0x10,0x01,0x00,0x00,0x00} ;
char udp_head_get_order[8]={0x4D,0x4B,0x01,0x11,0x01,0x00,0x00,0x00} ;

char tcp_head_position[8]={0x4D,0x4B,0x01,0x01,0x01,0x00,0x00,0x00} ;
char tcp_head_order[8] = {0x4D,0x4B,0x01,0x0F,0x01,0x00,0x00,0x00} ;
char tcp_head_num[8] = {0x4D,0x4B,0x01,0x02,0x01,0x00,0x00,0x00} ;
char tcp_head_to[8] = {0x4D,0x4B,0x01,0x03,0x01,0x00,0x00,0x00};
char tcp_head_back[8] = {0x4D,0x4B,0x01,0x04,0x01,0x00,0x00,0x00};
int order_num = 0 ;
int tcp_to = 0 ;
int tcp_back = 0 ;


char end[3]={0};
char mid[4]={'\0','\0','\0','\0'};
int udp_data_to_fy[20][3] = {{0,0,0},{0,0,0},{0,0,0}} ;
int udp_id = 0 ;
int udp_num = 0 ;
int udp_order = 0 ;
void set_posxyz(short x,short y,short z,char *out,int *len) {

	int i=0 ;
	i = 7 ;
	memcpy(out,head_posxyz,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	memcpy(out+i,&x,2) ;
	i += 2 ;
	memcpy(out+i,&y,2) ;
	i += 2 ;
	memcpy(out+i,&z,2) ;
	i += 2 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr1_start(float start_height,float speed,char *out,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_order,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = 0x01 ;
	i += 1 ;
	memcpy(out+i,&start_height,4)  ;
	i += 4 ;
	memcpy(out+i,&speed,4) ;
	i += 4 ;
	i += 20 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr2_angle(float angle,char *out ,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_order,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = 0x02 ;
	i += 1 ;
	memcpy(out+i,&angle,4)  ;
	i += 4 ;
	i += 24 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr3_to_position(float x,float y,float z,float speed,char *out,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_order,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = 0x03 ;
	i += 1 ;
	memcpy(out+i,&x,4)  ;
	i += 4 ;
	memcpy(out+i,&y,4) ;
	i += 4 ;
	memcpy(out+i,&z,4) ;
	i += 4 ;
	memcpy(out+i,&speed,4) ;
	i += 4 ;
	i += 12 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr4_down_speed(float speed,char *out,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_order,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = 0x04 ;
	i += 1 ;
	memcpy(out+i,&speed,4)  ;
	i += 4 ;
	i += 24 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr5_get(char *out,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_order,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = 0x05 ;
	i += 1 ;
	i += 28 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void set_contr6_line(char line,char *out,int *len) {
	int i = 0 ;
	i=7 ;
	memcpy(out,head_line,i) ;
	memcpy(out+i,mid,4) ;
	i += 4 ;
	out[i] = line ;
	i += 1 ;
	check(out,i,end) ;
	memcpy(out+i,end,2) ;
	i += 2 ;
	*len = i ;
}
void get_contr1_xyz(char *in,int len,int *x,int*y,int*z) {
	int i=7 ;
	if(!memcmp(in,head_getxyz,7)){
		i+=4 ;
		memcpy(x,in+i,4) ;
		i+=4 ;
		memcpy(y,in+i,4) ;
		i+=4 ;
		memcpy(z,in+i,4) ;
	}
}
void check(char *in,int len,char *out) {
	int i = 0 ;
	short d = 0 ;
	for(i=0;i<len;i++){
		d += in[i]&0xff ;
	}
	memcpy(out,&d,2) ;
}
void udp_xyz(int x,int y,int z,char *out,int *len) {
	int i=0 ;
	i=8 ;
	memcpy(out,udp_head_xyz,i) ;
	memcpy(out+i,&x,4) ;
	i += 4 ;
	memcpy(out+i,&y,4) ;
	i += 4 ;
	memcpy(out+i,&z,4) ;
	i += 4 ;
	*len = i ;
}

void udp_start_end(char *in,int len,int *x1,int *y1,int *z1,int *x2,int *y2,int *z2,int *flag/*err-1,ok1*/) {
	if(len!=32){
		*flag = -1 ;
		return ;
	}
	int i=0 ;
	i = 8 ;
	if(!memcmp(in,udp_head_order,8)){
		memcpy(x1,in+i,4) ;i += 4 ;
		memcpy(y1,in+i,4) ;i += 4 ;
		memcpy(z1,in+i,4) ;i += 4 ;
		memcpy(x2,in+i,4) ;i += 4 ;
		memcpy(y2,in+i,4) ;i += 4 ;
		memcpy(z2,in+i,4) ;
		*flag = 1 ;
	}else
		*flag = -1 ;
}
void get_posxyz(char *buf,int len,int *x,int *y,int *z,int *flag) {
	int i = 0 ;
	i = 7 ;
	if(!memcmp(buf,head_getxyz,i)){
		memcpy(x,buf+i,4);i += 4 ;
		memcpy(y,buf+i,4);i += 4 ;
		memcpy(z,buf+i,4);
		*flag = 1 ;
	}else
		*flag = -1 ;
}
void udp_set_xyz(char *in,int len,int out[][3],int *id,int *num) {
	int i=0 ,j=0;
	i = 8 ;
	if(!memcmp(in,udp_head_set_xyz,8)){
		*id = in[i]&0xff ;
		i++ ;
		*num = in[i]&0xff ;
		i++ ;
		for(j=0;j<*num ;j++){
			if(i>len)
				*num = 0 ;
			out[j][0] = *((int*)(in+i));i += 4 ;
			out[j][1] = *((int*)(in+i));i += 4 ;
			out[j][2] = *((int*)(in+i));i += 4 ;
		}
	}
}
void udp_get_order(char *in,int len,int *id,int *order) {
	int i=0 ;
	i = 8 ;
	if(!memcmp(in,udp_head_get_order,8)){
		*order = in[i]&0xff ;
		i++ ;
		*id = in[i]&0xff ;
	}
}
void tcp_xyz(int x,int y,int z,char *out,int *len) {
	int i=0 ;
	i=8 ;
	memcpy(out,tcp_head_position,i) ;
	memcpy(out+i,&x,4) ;
	i += 4 ;
	memcpy(out+i,&y,4) ;
	i += 4 ;
	memcpy(out+i,&z,4) ;
	i += 4 ;
	*len = i ;
}
void tcp_num(char *buf,int *num) {
	if((buf[3]&0xff) == 0x02){
		memcpy(num,buf+8,4) ;
		return ;
	}
	*num = -1 ;
	return ;
}
void tcp_fun_to(char *buf,int *num,int *to) {
	if((buf[3]&0xff) == 0x03){
		memcpy(num,buf+8,4) ;
		*to = (buf[12]&0xff) ;
		return ;
	}
	*num = -1 ;
	*to = -1 ;
	return ;
}
void tcp_fun_back(char *buf,int *num,int *back) {
	if((buf[3]&0xff) == 0x04){
		memcpy(num,buf+8,4) ;
		*back = (buf[12]&0xff) ;
		return ;
	}
	*num = -1 ;
	*back = -1 ;
	return ;
}
