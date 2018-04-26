/*
 * log.c
 *
 *  Created on: Nov 16, 2017
 *      Author: hxf
 */
#include <stdio.h>
#include "log.h"

int debug = 4 ;
int log_name_flag=3 ;
int log_name_time=2 ;
char stdlog[30]="./" ;
FILE *fp ;
int log_fi_name(){
	if(log_name_flag == 3)
	{
		memcpy(stdlog+2,ctime_buf,16) ;
		memcpy(stdlog+18,".txt",4) ;
		if(fp!=NULL)
			fclose(fp) ;
		fp = fopen(stdlog,"wb") ;
		if(fp!=NULL)
			return 1;
		else{
			fclose(fp) ;
			return -1 ;
		}
	}
	return 1 ;
}
int  log_file_re_name(){
		memcpy(stdlog+2,ctime_buf,16) ;
		memcpy(stdlog+18,".txt",4) ;
		if(fp!=NULL)
			fclose(fp) ;
		fp = fopen(stdlog,"ab") ;
		if(fp!=NULL)
			return 1;
		else{
			fp = fopen(stdlog,"wb") ;
			if(fp != NULL)
				return 1 ;
				fclose(fp) ;
			return -1 ;
		}
}

void dprintf1(char *format,...){
	if(log_name_flag == 3){
		log_fi_name() ;
		log_name_flag = 2 ;
	}
	if(fp != NULL){
		va_list ap ;
		va_start(ap,format) ;
		vfprintf(fp,format,ap) ;
		fflush(fp) ;
		va_end(ap) ;
	}else{
		log_file_re_name() ;
		if( fp != NULL){
			va_list ap ;
			va_start(ap,format) ;
			vfprintf(fp,format,ap) ;
			fflush(fp) ;
			va_end(ap) ;
		}
	}
}
