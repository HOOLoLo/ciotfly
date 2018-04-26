/*
 * unidimensional.h
 *
 *  Created on: Nov 19, 2017
 *      Author: root
 */

#ifndef UNIDIMENSIONAL_H_
#define UNIDIMENSIONAL_H_

extern int avg_d[10] ;
extern int avg_index ;
extern int width_length ;
void unidimensional_data() ;
int unidimensional_two_one(int input[][4]/*line x,y,z,d*/,int onepoint[],int output[],int height) ;


int set_one_info(int type,char *mac,int bx,int *out,int height)  ;
#endif /* UNIDIMENSIONAL_H_ */
