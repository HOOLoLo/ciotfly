/*
 * points.h
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */

#ifndef POINTS_H_
#define POINTS_H_
extern short point_short[20][6] ;//x,y,分组号，计算方法（一维，二维），灯管颜色，1,蓝色,2，紅
extern short ab_point[20][2] ;
extern short point_num ;
extern short current_point ;
extern int direction_i ;//1 pre,2,back
extern int dimension_i ;//1,一维,2,二维
extern int process_start ;//0 stop,1 start
int read_points(int num) ;
int judge_position(int x,int y,int direction,int pointi,int bias) ;
#endif /* POINTS_H_ */
