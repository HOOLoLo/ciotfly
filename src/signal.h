/*
 * signal.h
 *
 *  Created on: Oct 20, 2017
 *      Author: root
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_
extern char head_posxyz[8] ;
extern char head_order[8] ;
extern char head_getxyz[8] ;
extern char head_line[8] ;
extern char udp_head_xyz[8] ;
extern char udp_head_order[8] ;
extern char udp_head_set_xyz[8] ;
extern char udp_head_get_order[8] ;
extern int udp_data_to_fy[20][3] ;

extern char tcp_head_position[8] ;
extern char tcp_head_order[8] ;
extern char tcp_head_num[8] ;
extern char tcp_head_to[8] ;
extern char tcp_head_back[8] ;
extern int order_num ;
extern int tcp_to ;
extern int tcp_back ;

extern int udp_id ;
extern int udp_num ;
extern int udp_order ;
void set_posxyz(short x,short y,short z,char *out,int *len) ;
void set_contr1_start(float start_height,float speed,char *out,int *len) ;
void set_contr2_angle(float angle,char *out ,int *len) ;
void set_contr3_to_position(float x,float y,float z,float speed,char *out,int *len) ;
void set_contr4_down_speed(float speed,char *out,int *len) ;
void set_contr5_get(char *out,int *len) ;
void set_contr6_line(char line,char *out,int *len) ;
void get_contr1_xyz(char *in,int len,int *x,int*y,int*z) ;
void check(char *in,int len,char *out) ;

void get_posxyz(char *buf,int len,int *x,int *y,int *z,int *flag) ;


void udp_xyz(int x,int y,int z,char *out,int *len) ;
void udp_start_end(char *in,int len,int *x1,int *y1,int *z1,int *x2,int *y2,int *z2,int *flag/*err-1,ok1*/) ;
void udp_set_xyz(char *in,int len,int out[][3],int *id,int *num) ;
void udp_get_order(char *in,int len,int *id,int *order) ;


void tcp_xyz(int x,int y,int z,char *out,int *len) ;
void tcp_num(char *buf,int *num) ;
void tcp_fun_to(char *buf,int *num,int *to) ;
void tcp_fun_back(char *buf,int *num,int *back) ;
#endif /* SIGNAL_H_ */
