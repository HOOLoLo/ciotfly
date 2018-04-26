/*
 * unidimensional.c
 *
 *  Created on: Nov 19, 2017
 *      Author: root
 */
#include <math.h>
#include "unidimensional.h"
#include "analy.h"
#include "client_tcp.h"
void unidimensional_data() {
	int i=0 ;
	for(i=0 ;i<10;i++){
		avg_d[i]=0 ;
	}
	avg_index = 0 ;
	width_length = 0 ;
}
int unidimensional_two_one(int input[][4]/*line x,y,z,d*/,int onepoint[],int output[],int height) {
	int x = 0,y = 0 ;
	int d1 = 0  ,d2 = 0 ,d = 0 ,xd=0;
	int xd1=0 ,xd2=0 ;
	int dd = 370 ;
	int hx=0,hy=0 ;

	if(abs(input[0][0]-input[1][0])<2){
		x = input[0][0] ;
		y = abs(input[0][1]-input[1][1]) ;
		d1 = input[0][3] ;
		d2 = input[1][3] ;
		xd1 = (d1*d1+y*y-d2*d2)/(2*y) ;

		xd2 = sqrt(abs(pow(d1,2)-pow(xd1,2))) ;
		if(input[0][1]>input[1][1]){
			y = input[0][1]-xd1 ;
		}else if(input[0][1]<input[1][1]){
			y = input[0][1]+xd1 ;
		}
		hy = abs(onepoint[1]-y) ;
//		hx = sqrt(pow(onepoint[3],2)-pow((dd-height),2)-pow(hy,2)) ;
		hx = sqrt(pow(onepoint[3],2)-pow(xd2,2)-pow(hy,2)) ;
		if(onepoint[0]>x){
			x = onepoint[0]-hx ;
		}else if(onepoint[0]<x){
			x = onepoint[0]+hx ;
		}
	}else if(abs(input[0][1]-input[1][1])<2){
		y = input[0][1] ;
		x = abs(input[0][0]-input[1][0]) ;
		d1 = input[0][3] ;
		d2 = input[1][3] ;
		xd1 = (d1*d1+x*x-d2*d2)/(2*x) ;
		if(d1<d2){
			xd2 = sqrt(abs(pow(d1,2)-pow(xd1,2)));
		}else if(d1>d2){
			xd2 = sqrt(abs(pow(d2,2)-pow((x-xd1),2))) ;
		}
		if(xd2>dd-35)
			xd2 = dd/2 ;
		xd2 = dd- 100;
//		printf("height %d,%d\n",xd2,dd-height) ;
		if(input[0][0]>input[1][0]){
			x = input[0][0]-xd1 ;
		}else if(input[0][0]<input[1][0]){
			x = input[0][0]+xd1 ;
		}

		hx = abs(onepoint[0]-x) ;
		hy = sqrt(abs(pow(onepoint[3],2)-pow(xd2,2)-pow(hx,2))) ;
//		printf("hy2 %d\n",hy) ;
		if(onepoint[1]>y){
			y = onepoint[1]-hy ;
		}else if(onepoint[1]<y){
			y = onepoint[1]+hy ;
		}

	}else{
		output[0]=0 ;
		output[1]=0 ;
		output[1]=0 ;
		return -1;
	}
	output[0] = x ;
	output[1] = y ;
	output[2] = height ;

	return 0 ;
}

int set_one_info(int type,char *mac,int bx,int *out,int height)  {
	//printf("----------------%d,one_point %d\n",point_num_current,one_point_num) ;
	int i = 0,ret=0 ;
	int cx = 2;//2 ;
	if(type == 0xf1){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		if(point_num_current+one_point_num>=3){
			if((ret=unidimensional_two_one(one_inputData,one_inputData[10],one_output,height))==0){
				*out = 0xff ;
			}
		}
		point_num_current = 0 ;
		one_point_num = 0 ;
		return 0 ;
	}else if(type == 1){
	//	printf("-----------------------------------------------%d\n",point_num_current) ;
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		if(point_num_current+one_point_num>=3){
//			point_num_current = 3 ;
			if((ret=unidimensional_two_one(one_inputData,one_inputData[10],one_output,height))==0){
				*out = 0xff ;
			}
		}
		point_num_current = 0 ;
		one_point_num = 0 ;
		for(i=0;i<point_num_all;i++){
	//		printf("gpoint id %d\n",gpoint[i].id) ;
			if(gpoint[i].id != 2){
				if(!memcmp(mac,gpoint[i].mac,8)){
					if(gpoint[i].id == 1){
						one_inputData[point_num_current][0] = gpoint[i].xx ;
						one_inputData[point_num_current][1] = gpoint[i].yy ;
						one_inputData[point_num_current][2] = gpoint[i].zz ;
						one_inputData[point_num_current][3] = bx ;
						point_num_current++ ;
	//					printf("point_num_current %d-----------\n",point_num_current) ;
						break ;
					}else if(gpoint[i].id == 3){
						one_inputData[10][0] = gpoint[i].xx ;
						one_inputData[10][1] = gpoint[i].yy ;
						one_inputData[10][2] = gpoint[i].zz ;
						one_inputData[10][3] = bx ;
						one_point_num = 1 ;
	//					printf("one_point_num %d-----------\n",one_point_num) ;
						break ;
					}
				}
			}
		}
	}else if(type == 0){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		for(i=0;i<point_num_all;i++){
	//		printf("gpoint id %d\n",gpoint[i].id) ;
			if(gpoint[i].id != 2){
				if(!memcmp(mac,gpoint[i].mac,8)){
					if(gpoint[i].id == 1){
						one_inputData[point_num_current][0] = gpoint[i].xx ;
						one_inputData[point_num_current][1] = gpoint[i].yy ;
						one_inputData[point_num_current][2] = gpoint[i].zz ;
						one_inputData[point_num_current][3] = bx ;
						point_num_current++ ;
	//					printf("point_num_current %d-----------\n",point_num_current) ;
						break ;
					}else if(gpoint[i].id == 3){
						one_inputData[10][0] = gpoint[i].xx ;
						one_inputData[10][1] = gpoint[i].yy ;
						one_inputData[10][2] = gpoint[i].zz ;
						one_inputData[10][3] = bx ;
						one_point_num = 1 ;
	//					printf("one_point_num %d-----------\n",one_point_num) ;
						break ;
					}
				}
			}
		}
		if(point_num_current+one_point_num>=3){
	//		printf("--#############---%d,%d\n",point_num_current,one_point_num) ;
			if((ret=unidimensional_two_one(one_inputData,one_inputData[10],one_output,height))==0){
				*out = 0xff ;
			}
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
	}
	return -1 ;
}
