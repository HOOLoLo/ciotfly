/*
 * ancom.h
 *
 *  Created on: Oct 18, 2017
 *      Author: root
 */

#ifndef ANCOM_H_
#define ANCOM_H_
#include <sys/socket.h>
#include <netinet/in.h>
//int com_send(char *buf,int len) ;
int com_recv(char *buf,int len) ;

void cetc_create_com_thread() ;

extern pthread_mutex_t mutex_send;

extern char send_buf[100] ;
char m_send_buf[100] ;
int  m_send_len ;
extern int send_len ;
extern int send_flag ;

void com_send_stop() ;
void com_send_group(int id) ;
void com_send_start() ;
#endif /* ANCOM_H_ */
