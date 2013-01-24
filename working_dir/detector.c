#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "cv.h"
#include "highgui.h"
#include<time.h>

void processImage(IplImage* image);
int somethingIsMoving(void);
void runDetection();
int onChange();

CvCapture* capture;
IplImage* absolute_diff_img;
CvMat* previous_img;
CvMat* average_img;
CvMat* gray_img;
int isFirstTime, i_width, i_height;
CvSeq* currentContours;
int currentSurface = 0;
int surface = 1;
int threshold = 3;
int main(int argc, char** argv) {
	capture = cvCaptureFromCAM(0); // capture from video device #0

	isFirstTime = 1;
	/*time_t rawtime = time(NULL);
	struct tm*  timeinfo;
	char *s=ctime(&rawtime);
	//time(&timeinfo);
	timeinfo = localtime(&rawtime);
	(*timeinfo).tm_year += 5;
	int x = strcmp(s, asctime(timeinfo));
	*/
	time_t sec1;
	sec1 = time (NULL);
	time_t sec2;
	sec2 = time (NULL);
	printf ( "Current local time and date: %i and %i \n", sec1, sec2 );

	if (!cvGrabFrame(capture)) { // capture a frame
		printf("Could not grab a frame\n\7");
		exit(0);
	}

	IplImage* img = cvQueryFrame(capture);
	i_height = img->height;
	i_width = img->width;
	gray_img = cvCreateMat(i_height, i_width, CV_8U);
	cvCvtColor(img, gray_img, CV_RGB2GRAY);
	average_img = cvCreateMat(i_height, i_width, CV_8U);
	previous_img = cvCreateMat(i_height, i_width, CV_8U);

	surface = i_height * i_width;
	cvNamedWindow("Detected Image", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Detection", "Detected Image", &threshold, 100, NULL);

	runDetection();
}

void processImage(IplImage* image) {

	cvCvtColor(image, previous_img, CV_RGB2GRAY);
	cvAbsDiff(gray_img, previous_img, average_img);
	cvSmooth(average_img, average_img, CV_BLUR, 5, 5, 0, 0);
	cvMorphologyEx(average_img, average_img, NULL, NULL, CV_MOP_OPEN, 1);
	cvMorphologyEx(average_img, average_img, NULL, NULL, CV_MOP_CLOSE, 1);
	cvThreshold(average_img, average_img, 10, 255, CV_THRESH_BINARY_INV);
}

int somethingIsMoving(void) {

	int no_black_pixles = 0;
	int i = 0;
	int j = 0;
	for(; i < i_height; i++) {
		for (; j < i_width; j++) {
			//printf("Bp = %f\n", CV_MAT_ELEM( *average_img, float, i, j ));
			if( (CV_MAT_ELEM( *average_img, double, i, j )) == 0.0)
				//printf("Bp=== = %d\n", no_black_pixles);
				no_black_pixles += 60;
		}
	}
	//printf("Bp=== = %d\n", no_black_pixles);
	int averge_black_pixels = (no_black_pixles * 1000)/surface;
	//printf("Black = %d\n", averge_black_pixels);
	if(averge_black_pixels > threshold)
		return 1;
	else
		return 0;
}
int onChange(){
	return 5;
}
void runDetection(){
	int i = 0;
	while (1) {
		IplImage* currentImg = cvQueryFrame(capture);
		processImage(currentImg);
		if (somethingIsMoving()){
			printf("Hello Something is moving!!! %i\n", i++);
			printf("\r\n\n");
		}
		cvDrawContours(currentImg, currentContours, cvScalarAll(255),cvScalarAll(255), 1, 2, 8, cvPoint(0,0));
		cvShowImage("Detected Image", currentImg);
		cvShowImage("Res", average_img);
		cvCopy(previous_img, gray_img, NULL);
		int key = cvWaitKey(2);
		if (key == 27 || key == 10)
			break;

	}
		//cvDrawContours(currentImg, self.currentcontours, (0, 0, 255), (0, 255, 0), 1, 2, cv.CV_FILLED);
}

