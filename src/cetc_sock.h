/*
 * cetc_sock.h
 *
 *  Created on: Jan 13, 2017
 *      Author: root
 */

#ifndef CETC_SOCK_H_
#define CETC_SOCK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "analy.h"
#include "signal.h"
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
extern int sockfd ;
extern struct sockaddr_in servaddr ;

extern int init_flag ;


int udp_client_init(IN char *ip,IN int port) ;
int udp_init() ;
int udp_close() ;
int udp_sockopt(IN int t);
int udp_send(char *buf,int len)  ;
void* thread_sock_send(void *arg) ;
void* thread_sock_recv(void *arg) ;

void cetc_create_thread() ;
#endif /* CETC_SOCK_H_ */
