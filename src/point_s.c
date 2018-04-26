/*
 * points.c
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stddef.h>
#include "point_s.h"


//读取转角点信息数据
int read_points(int num) {
	FILE *file ;
	char finame[50]={0} ;//这里用={0}没用吧

	sprintf(finame,"/root/points-%02d.ini",num) ;//打开不同的points设置文件。
	char *key = "NUM";
	file = fopen(finame,"rb") ;//“rb”以二进制形式读写
	if(file == NULL)
		return -1 ;
	char buf[1024] ;
	char str[300] ;
	int i=0 ;
	int j=0 ;
	int k=0 ;

	memset(buf,'\0',1024) ;
	memset(str,'\0',300) ;

	//一行一行的找，找到文件中一行的前三个字符是Key后把这行赋值给buff跳出循环，否则把buf全部赋值为‘\0’这个是字符串结尾符
	while(fgets(buf,1024,file)){
		if(!memcmp(buf,key,3)){
			break ;
		}
		memset(buf,'\0',1024) ;
	}

	i = 4 ;
	j=0 ;
	//把“NUM=”后面的数字放到str中
//这个操作有必要吗直接用下面的atoi就可以了啊；应该是为了以后如果前面要加数字什么的考虑吧，方便以后扩展
	while(buf[i]!='\n'){
		str[j] = buf[i] ;
		i++,j++ ;
	}

//atoi把字符串转换成整型的函数 atoi（char *nptr）函数会扫描参数 nptr字符串，跳过
// 前面的空白字符（例如空格，tab缩进）等，可以通过isspace( )函数来检测），直到遇上
// 数字或正负符号才开始做转换，而在遇到非数字或字符串结束时('\0')才结束转换，并将结果
// 返回。如果 nptr不能转换成 int 或者 nptr为空字符串，那么将返回 0


	//这里把str里面的数字付给point_num

	point_num = atoi(str) ;
	//point_num为什么不能大于20;？因为前面开的point_short数组只有20；
	if(point_num>20){
		point_num = 0 ;
		return -1 ;
	}

	//把转角数据传到
	for(k=0;k<point_num;k++){
		memset(buf,'\0',1024) ;
		if(fgets(buf,1024,file)){//
			i = 0 ;
			while(buf[i]!='\n'){
				if(buf[i]=='('&&buf[i]!='\n'){
					i++ ;
					break ;
				}
				i++ ;
			}
			j=0;
			memset(str,'\0',300) ;
			while(buf[i]!=','&&buf[i]!='\n'){//x
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][0] = atoi(str) ;
			j=0 ;i++ ;
			memset(str,'\0',300) ;
			while(buf[i]!=','&&buf[i]!='\n'){//y
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][1] = atoi(str) ;

			j=0 ;i++ ;
			memset(str,'\0',300) ;
			while(buf[i]!=','&&buf[i]!='\n'){//group
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][2] = atoi(str) ;

			j=0 ;i++ ;
			memset(str,'\0',300) ;
			while(buf[i]!=','&&buf[i]!='\n'){//w
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][3] = atoi(str) ;

			j=0 ;i++ ;
			memset(str,'\0',300) ;
			while(buf[i]!=','&&buf[i]!='\n'){//rgb
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][4] = atoi(str) ;

			j=0 ;i++ ;
			memset(str,'\0',300) ;
			while(buf[i]!=')'&&buf[i]!='\n'){//(0x,y0)
				str[j]=buf[i];
				i++ ,j++ ;
			}
			point_short[k][5] = atoi(str) ;
//			printf("%d,%d,%d,%d,%d\n",point_short[k][0],point_short[k][1],point_short[k][2],point_short[k][3],point_short[k][4]);
		}
	}
	fclose(file) ;
	return 1 ;
}



//判断是否到达下一个位置
int judge_position(int x,int y,int direction,int pointi/*0,1,2,3,4...*/,int bias) {//return 1,next
//	int i=0 ,max=0,min=0;
//	printf("x %d,y %d,dir %d,point %d,bias %d\n",x,y,direction,pointi,bias) ;
//	if(direction==1){

		if(point_short[pointi][0]==point_short[pointi+1][0]){
//			printf("x=x,y=%d\n",y) ;
			if(point_short[pointi][1]<point_short[pointi+1][1]){
				if(point_short[pointi+1][1]<y){
					return 1 ;
				}
			}else if(point_short[pointi][1]>point_short[pointi+1][1]){
				if(point_short[pointi+1][1]>y){
					return 1 ;
				}
			}

		}else if(point_short[pointi][1] == point_short[pointi+1][1]){
//			printf("y=y x=%d\n",x) ;
			if(point_short[pointi][0]<point_short[pointi+1][0]){
				if(x>point_short[pointi+1][0])
					return 1 ;
			}else if(point_short[pointi][0]>point_short[pointi+1][0]){
				if(x<point_short[pointi+1][0])
					return 1 ;
			}

		}
//	}
	return -1 ;
}
