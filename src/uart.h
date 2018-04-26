/*
 * uart.h
 *
 *  Created on: Oct 12, 2017
 *      Author: root
 */

#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
//宏定义
#define FALSE  -1
#define TRUE   0
//extern pthread_mutex_t mutex_serial ;
int UART0_Open(int fd,char* port);
void UART0_Close(int fd) ;
int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity) ;
int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity) ;
int UART0_Recv(int fd, char *rcv_buf,int data_len) ;
int UART0_Send(int fd, char *send_buf,int data_len) ;
//void serial_read(char *com,int para,char *buf,int *len) ;
//void serial_send(char *com,int para,char *buf,int len) ;
void serial_send_stop(char *com,int para) ;
void serial_send_start(char *com,int para) ;
void serial_send_group(char *com,int para,int id) ;
#endif /* UART_H_ */
