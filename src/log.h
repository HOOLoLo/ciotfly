/*
 * log.h
 *
 *  Created on: Nov 16, 2017
 *      Author: hxf
 */

#ifndef LOG_H_
#define LOG_H_

#include <time.h>
#include <stdarg.h>
#define L_LOG 3
#define DP_PREFIX "%s "
#define DP_LOG_TEXT "LOG:"
#define DP_LOG_PREFIX DP_PREFIX DP_LOG_TEXT
#define is_printable(_level) ((debug)>=(_level))
extern int debug ;

#define MY_DPRINT0(_prefix,_fmt,args...)\
		dprintf1(_prefix _fmt,dp_time(),##args);
#define MY_LOG(fmt,args...)\
	do{\
		if(is_printable(L_LOG)){\
			MY_DPRINT0( DP_LOG_PREFIX, fmt, ##args);\
			}\
	}while(0)

#define MY_LOG_1_( fmt, args...) \
				do { \
					if (is_printable(L_LOG)){ \
							MY_DPRINT0( DP_LOG_PREFIX, fmt, ##args);\
					} \
				}while(0)
char ctime_buf[20] ;
extern int log_name_flag ;
extern int log_name_time ;
static inline char* dp_time(void)
{
	time_t ltime;
	struct tm * timeinfo;
	time(&ltime);
	timeinfo = localtime ( &ltime );
	//ctime_r( &ltime, ctime_buf);
	sprintf(ctime_buf,"%04d-%02d-%02d %02d:%02d:%02d",\
			 1900+timeinfo->tm_year,1+timeinfo->tm_mon,\
			 timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	ctime_buf[19] = 0; /* remove year*/
	if(log_name_flag == 0){
		log_name_time = timeinfo->tm_mday ;
		log_name_flag = 2 ;
	}
	if(log_name_time != timeinfo->tm_mday){
		log_name_time = timeinfo->tm_mday ;
		log_name_flag = 3 ;
	}
	//return ctime_buf;  /* remove name of day*/
	return NULL ;
}
void dprintf1(char *format,...);


#endif /* LOG_H_ */

