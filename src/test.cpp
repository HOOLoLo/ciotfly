#include "get_light.h"
//#define _GNU_SOURCE
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <sched.h>
extern int pix_x;
extern int pix_y;
using namespace std;
using namespace cv;
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

static pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;

int thread_count=0;

struct arg_thread
{
	uchar red_L[3];
	uchar red_H[3];
	Point2i core;
	int theta;
	int tag=0;

};
const int mthreshold=10;
Mat pframe;

void mutex_unlock(void * p){
	pthread_mutex_unlock((pthread_mutex_t *)p);
	return ;
}

void * red_light_thread(void * arg)
{	
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(1,&mask);
	if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask)<0)
		{
			fprintf(stderr,"set thread affinity failed\n");
		}



	arg_thread* arg_T = (arg_thread*)arg;
	pthread_cleanup_push(&mutex_unlock, (void*)&mutex);
	Point2i tophead;
	Point2i bottomhead;
	Point2i toptail;
	Point2i bottomtail;
	Point2i top;
	Point2i bottom;
	Point2i core = Point2i(0, 0);
	Mat hsv;
	Mat dst=Mat(480,640,CV_8UC1);
	double theta;
	while (1) {
		cout << "thread is running" << endl;
		pthread_mutex_lock(&mutex);
		cout<<"thread locked the mutex"<<endl;

		thread_count++;
		cout<<"thread_count:"<<thread_count<<endl;

		pthread_cond_wait(&cond, &mutex);

		cout<<"thread process start"<<endl;
		cvtColor(pframe, hsv, COLOR_BGR2HSV);
		inrange_c(hsv, arg_T->red_L, arg_T->red_H, dst);
		arg_T->theta = find_true_theta(dst, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);
		arg_T->core = find_core(top, bottom, dst.cols, dst.rows);
		cout << "done" << endl;
		arg_T->tag=1;	
	
	//	pthread_mutex_lock(&mutex1);

	//	pthread_cond_signal(&cond1);
//
	//	pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex);
	}
	pthread_cleanup_pop(0);
	pthread_exit(0);
}

int main() {
	cpu_set_t m_mask;	
	CPU_ZERO(&m_mask);
	CPU_SET(2,&m_mask);
	if (pthread_setaffinity_np(pthread_self(),sizeof(m_mask),&m_mask)<0)
	{
		fprintf(stderr,"set thread affinity failed\n");
	}



	pthread_t tid;
	ofstream ofile("data.txt");
	int iSend, iRecv;
	int sock;
	char msg[22] = { 0x4D,0x4B,0x01,0x06,0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	char rec_buff[512];
	Mat hsv;
	Mat gray;
	Mat gray2;
	Mat fingray;
	Point2i core3 = Point2i(0, 0);

	int HvL = 115;
	int SvL = 200;
	int VvL = 100;
	int HvH = 125;
	int SvH = 255;
	int VvH = 255;
	uchar red_L[3] = { 170,150,20 };
	uchar red_H[3] = { 180,255,255 };
	uchar blue_L[3] = { 115,230,100 };
	uchar blue_H[3] = { 125,255,255 };

	arg_thread arg{ { red_L[0],red_L[1],red_L[2] },{ red_H[0],red_H[1],red_H[2] },core3 };

	uchar table_H[256];
	uchar table_S[256];
	uchar table_V[256];
	pthread_create(&tid, NULL,red_light_thread,(void *) &arg);


	VideoCapture cap(200);
	if (!cap.isOpened())
		cout << "camera error" << endl;
	//if(cap.set(CV_CAP_PROP_EXPOSURE,-1)<0)
	//	cout<<"set error"<<endl;
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))<0)
	{
		perror("socket error");
		return 1;
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(23456);
	server_addr.sin_addr.s_addr = inet_addr("193.168.1.100");
	socklen_t len = sizeof(server_addr);
	//	if(bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0){
	//	perror("bind error");
	//	return 1;

	//	}
	//	sin_size=sizeof(struct sockaddr_in);

	//char buf[1024];

	//while(1){

	//	int r=recvfrom(sever_sockfd,buf,sizeof(buf)-1,0,);
	//	if(r<0){
	//	perror("recvfrom");
	//	exit(3);
	//	}
	//	else{
	Mat frame;
	int colortag = 1;
	Size nsize = Size(640, 480);
	//	VideoWriter writer=VideoWriter("video.avi",CV_FOURCC('M','J','P','G'),30,nsize,true);
	int color_count = 0;
	while (cap.isOpened()) {
		double t = (double)getTickCount();
		while (!cap.read(frame)){
			cap.read(frame);
			cout<<"reading camera...."<<endl;
		}

		if (pthread_mutex_lock(&mutex) == 0) {
			cout<<"main thread locked the mutex"<<endl;
			while (!cap.read(pframe)){
				cap.read(pframe);
				cout<<"reading camera...."<<endl;
			}

			pthread_mutex_unlock(&mutex);
			cout<<"main thread released the mutex"<<endl;
		}
			while(1){
			if(thread_count>0){
			pthread_mutex_lock(&mutex);
				cout<<"signaling"<<endl;
				pthread_cond_signal(&cond);
				thread_count--;
			pthread_mutex_unlock(&mutex);
			break;
			}
			}
		
		
		Mat edgePic;
		Mat linePic;
		int nRows = frame.rows;
		int nCols = frame.cols;
		Point2i tophead;
		Point2i bottomhead;
		Point2i toptail;
		Point2i bottomtail;
		Point2i top;
		Point2i bottom;
		Point2i core = Point2i(0, 0);
		short int tag;
		double theta1 = err;
	
		Point2i tophead2;
		Point2i bottomhead2;
		Point2i toptail2;
		Point2i bottomtail2;
		Point2i top2;
		Point2i bottom2;
		Point2i core2 = Point2i(0, 0);
		double theta2 = err;
		Mat dst = Mat(frame.rows, frame.cols, CV_8UC1);
		Mat dst_change = Mat(frame.cols, frame.rows, CV_8UC1);
		Mat rotation = Mat(frame.cols, frame.rows, CV_8UC1);
		Mat rotate = (Mat_<float>(2, 3) << 0, 1, 0, 1, 0, 0);
		cvtColor(frame, hsv, COLOR_BGR2HSV);
	//	inRange(hsv,Scalar(HvL,SvL,VvL),Scalar(HvH,SvH,VvH),dst);
		inrange(hsv, blue_L, blue_H, dst );
		//Mat LUT= Mat(frame.rows, frame.cols, CV_8UC1);
		//�ò��ұ��õ��ĻҶ�ͼ
		//inrange_lut(hsv, table_H, table_S, table_V, LUT);
		//warpAffine(dst_change, rotation, rotate, Size(480, 640));

		theta1 = find_true_theta(dst, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);
		cout<<"theta1="<<theta1<<endl;
		//theta2 = find_true_theta(dst_change, mthreshold, tophead2, toptail2, bottomhead2, bottomtail2, top2, bottom2);

		//		if(color_count>0){

		//			colortag=3;

		//		}

		//cout<<"colorcount:"<<color_count<<endl;

		//cout<<colortag<<endl;

		/*if(colortag==1){

		HvL = 115;

		SvL = 230;

		VvL = 100;

		HvH = 125;

		SvH = 255;

		VvH = 255;

		inRange(hsv, Scalar(HvL, SvL, VvL), Scalar(HvH, SvH, VvH),fingray);

		theta1 = find_true_theta(fingray, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);

		}
		else if(colortag==2){
		HvL = 170;
		SvL = 160;
		VvL = 50;
		HvH = 180;
		SvH = 255;
		VvH = 180;

		inRange(hsv, Scalar(HvL, SvL, VvL), Scalar(HvH, SvH, VvH),fingray);

		theta2=Col_Search_theta(fingray, mthreshold, tophead2, toptail2, bottomhead2, bottomtail2, top2, bottom2);

		}

		else if(colortag==3){

		HvL = 115;

		SvL = 230;

		VvL = 100;

		HvH = 125;

		SvH = 255;

		VvH = 255;

		inRange(hsv, Scalar(HvL, SvL, VvL), Scalar(HvH, SvH, VvH),fingray);

		theta1 = find_true_theta(fingray, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);

		HvL = 170;

		SvL = 160;

		VvL = 50;

		HvH = 180;

		SvH = 255;

		VvH = 180;

		inRange(hsv, Scalar(HvL, SvL, VvL), Scalar(HvH, SvH, VvH),fingray);

		theta2=Col_Search_theta(fingray, mthreshold, tophead2, toptail2, bottomhead2, bottomtail2, top2, bottom2);

		}*/

		//time_t ltime;

		//struct tm * timeinfo;

		//time(&ltime);

		//timeinfo =localtime(&ltime);

		//ofile<<timeinfo->tm_mday<<" "<<timeinfo->tm_hour<<" "<<timeinfo->tm_min<<" "<<timeinfo->tm_sec<<" "<<endl;

		if (theta1 != err) {

			//	cout<<theta<<endl;

			core = find_core(top, bottom, frame.cols, frame.rows);

			//	if(core.x<fingray.cols/2){
			//		tag=1;
			//	}
			//	if(core.x>fingray.cols/2){
			//		tag=2;
			//	}

			//ofile<<1<<endl;

			//ofile<<core.x<<endl;

			cout << "Blue:" << "x:" << core.x << "y:" << core.y << endl;

			//	line(frame,top,bottom,Scalar(55,100,100),2);
			//	cout << top << bottom << endl;

			//	line(fingray, top, bottom, Scalar(55, 100, 100), 2, LINE_AA);

			//	cout << theta << endl;

			//	cout<<core.x<<endl;

			//	cout<<tag<<endl;

			//	imshow("Video Test", fingray);

			//
			//cout << theta << endl;

			//	if (waitKey(delay) >= 0)

			//		stop = true;

		}
	//	pthread_mutex_lock(&mutex1);
	//	cout<<"mutex1 is locked"<<endl;
	//	pthread_cond_wait(&cond1,&mutex1);
	while(1){
		if(arg.tag==1){
		if (arg.theta!=err) {
			//core2 = find_core(top2, bottom2, frame.cols, frame.rows);
			//ofile<<core2.y<<endl;
			theta2=arg.theta;
			core2=arg.core;
			cout << "red:" << "x:" << arg.core.x << "y:" << arg.core.y << endl;

		}
		arg.tag=0;
	//	pthread_mutex_unlock(&mutex1);
		break;
		}
		//	writer.write(frame);
	}
		//

		if (theta1 != err || theta2 != err) {

			if (colortag == 1 && theta1 != err) {

				

				short int msgt = (short int)(theta1 + 0.5);

				memcpy(msg + 11, &msgt, 2);

				msg[21] = 0x01;



			}

			else if (colortag == 2 && theta2 != err) {

				short int msgt = (short int)(theta2 + 0.5);

				memcpy(msg + 11, &msgt, 2);

				msg[21] = 0x02;

			}

			else if (colortag == 3) {

				if (theta1 != err) {

					short int msgt = (short int)(theta1 + 0.5);

					memcpy(msg + 11, &msgt, 2);



				}

				else if (theta2 != err) {
					short int msgt = (short int)(theta2 + 0.5);
					memcpy(msg + 11, &msgt, 2);
				}
				msg[21] = 0x03;
				//	color_count--;
			}
			short int msgCoreX = (short int)(core.x);

			short int msgCoreY = (short int)(core2.y);



			memcpy(msg + 17, &msgCoreX, 2);

			memcpy(msg + 19, &msgCoreY, 2);

			short int checknum = 0;

			for (int i = 0; i<15; i++) {

				checknum += msg[i];

			}

			memcpy(msg + 15, &checknum, 2);

			iSend = sendto(sock, msg, 22, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

			if (iSend<0)

			{

				perror("sendto");

			}

		}
		else {

			char nodata[1] = { 0x00 };

			iSend = sendto(sock, nodata, 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

			if (iSend<0)

			{

				perror("sendto");

			}

			cout << "success" << endl;

			//if(colortag==1)colortag=2;

			//else if(colortag==2)colortag=1;

			//cout<<"searching the light"<<endl;

			//cout<<colortag<<endl;

			//cout<<HvL<<endl;

		}

		iRecv = recvfrom(sock, rec_buff, sizeof(rec_buff), MSG_DONTWAIT, (struct sockaddr*)&server_addr, (socklen_t*)&len);

		if (iRecv >= 0) {

			cout << (rec_buff[0] & 0xff) << endl;

			if ((rec_buff[0] & 0xff) == 0x01)

			{

				colortag = 1;

				//	cout<<rec_buff[0];



			}

			else if ((rec_buff[0] & 0xff) == 0x02) {

				colortag = 2;

				//	cout<<rec_buff[0];

			}

			else if ((rec_buff[0] & 0xff) == 0x03) {

				colortag = 3;

				//	cout<<rec_buff[0];

				//	color_count=12;

			}

		}





		cout << colortag << endl;

		//	usleep(1);

	//		double	t2 = ((double)getTickCount() - t) / getTickFrequency();


		//		if(t2<0.1){

		//		usleep(1000*1000*(0.1-t2));

		//		}

		t = ((double)getTickCount() - t) / getTickFrequency();

		cout << "times passed in seconds: " << t << endl;
		//	if(iRecv<0){
		//	perror("recvfrom");
		//	}
		//	else if(iRecv==0){
		//	break;
		//	else{
		//		break
		//	}
	}
	//writer.release();	
	close(sock);
	cap.release();
	cin.get();
	return 0;
}
#include "get_light.h"
using namespace std;
