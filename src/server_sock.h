/*
 * server_sock.h
 *
 *  Created on: Nov 3, 2017
 *      Author: root
 */

#ifndef SERVER_SOCK_H_
#define SERVER_SOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "threadi.h"
#include "point_s.h"
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
int sock_serverfd ;
extern struct sockaddr_in serveraddr ;


int udp_server_client_init(IN char *ip,IN int port) ;
int udp_server_init() ;
int udp_server_close() ;
int udp_server_sockopt(IN int t);
void* thread_server_sock_send(void *arg) ;
void* thread_server_sock_recv(void *arg) ;
void* thread_server_sock_data(void *arg) ;
void* thread_server_sock_heart(void *arg) ;
void* thread_server_sock_com(void *arg) ;

void server_create_thread() ;
extern short i_lr ;
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex_log ;
extern char pix_buf[30] ;
extern int pix_flag ;
extern int pix_x ;
extern int pix_y ;
#endif /* SERVER_SOCK_H_ */
