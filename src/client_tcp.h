/*
 * client_tcp.h
 *
 *  Created on: Nov 19, 2017
 *      Author: root
 */

#ifndef CLIENT_TCP_H_
#define CLIENT_TCP_H_
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


struct sockaddr_in server_addr ;
int g_index_num ;
int g_index_send_num ;
int tcp_client_init(char *ip,int port) ;
int tcp_init() ;
int tcp_close() ;
int tcp_sockopt(int t);
int tcp_send(char *buf,int len) ;
int tcp_recv(char *buf,int *len) ;
void* thread_socktcp_init(void *arg);
void* thread_socktcp_send(void *arg);
void* thread_socktcp_recv(void *arg);
void tcp_create_thread() ;
extern int sock_fd ;
extern int connect_fdd ;
extern int sock_fd_flag  ;
#endif /* CLIENT_TCP_H_ */
