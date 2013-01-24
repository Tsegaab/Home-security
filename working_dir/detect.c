/*
 * detect.c
 *
 *  Created on: Jan 24, 2013
 *      Author: tekes
 *      Copyright (c) 2009 Adii Corporation \n All Right Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>

int initDetector();
void initRecorder();
void processImage(IplImage* image);
int somethingIsMoving(void);
void runDetection();
int onChange();

CvCapture* capture;
CvFont* font;
IplImage* img;
IplImage* absolute_diff_img;
CvMat* previous_img;
CvMat* average_img;
CvMat* gray_img;
CvVideoWriter *writer = 0;
int isFirstTime, i_width, i_height;
CvSeq* currentContours;
int currentSurface = 0;
int surface = 1;
int threshold = 3;
int recording = 0;
time_t detection_time = 0;

int main(int argc, char** argv) {
	if (!initDetector())
		exit(0);
	initRecorder();
	runDetection();
}
void initRecorder() {
	time_t today_time = time(NULL);
	//struct tm * timeinfo;
	char *s = ctime(&today_time);
	s[strlen(s) - 14] = s[strlen(s) - 5];
	s[strlen(s) - 13] = s[strlen(s) - 4];
	s[strlen(s) - 12] = s[strlen(s) - 3];
	s[strlen(s) - 11] = s[strlen(s) - 2];
	s[strlen(s) - 10] = '.';
	s[strlen(s) - 9] = 'w';
	s[strlen(s) - 8] = 'm';
	s[strlen(s) - 7] = 'v';
	s[strlen(s) - 3] = 0;
	s[strlen(s) - 2] = 0;
	s[strlen(s) - 1] = 0;
	writer = cvCreateVideoWriter(s, CV_FOURCC('M', 'J', 'P', 'G'), 5, cvGetSize(img), 1);
	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, 8);
}
int initDetector() {
	capture = cvCaptureFromCAM(0); // capture from video device #0

	isFirstTime = 1;
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Current local time and date: %s", asctime(timeinfo));

	if (!cvGrabFrame(capture)) { // capture a frame
		printf("Could not grab a frame\n\7");
		return 0;
	}

	img = cvQueryFrame(capture);
	i_height = img->height;
	i_width = img->width;
	gray_img = cvCreateMat(i_height, i_width, CV_8U);
	cvCvtColor(img, gray_img, CV_RGB2GRAY);
	average_img = cvCreateMat(i_height, i_width, CV_8U);
	previous_img = cvCreateMat(i_height, i_width, CV_8U);

	surface = i_height * i_width;
	cvNamedWindow("Detected Image", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Detection", "Detected Image", &threshold, 100, NULL);

	return 1;

}
void runDetection() {
	int i = 0;
	time_t started_time = time(NULL);
	while (1) {
		IplImage* currentImg = cvQueryFrame(capture);
		time_t now = time(NULL);
		processImage(currentImg);

		if (!recording) {
			if (somethingIsMoving()) {
				detection_time = now;
				if (now > (started_time + 5)) {
					printf("Something is moving!!! %i\n", i++);
					printf("\r\n\n");
					recording = 1;
				}
			}
		} else {
			if (now >= (detection_time + 10)) {
				printf("Recording ended %i\n", i);
				recording = 0;
			} else {
				time_t rec_time = time(NULL);
				//time ( &rawtime );
				struct tm * timeinfo = localtime( &rec_time);
				cvPutText(currentImg, asctime(timeinfo), cvPoint(25, 0), font, cvScalarAll(0));
				cvWriteFrame(writer, currentImg);
			}
		}
		cvDrawContours(currentImg, currentContours, cvScalarAll(255),
				cvScalarAll(255), 1, 2, 8, cvPoint(0, 0));
		cvShowImage("Detected Image", currentImg);
		cvShowImage("Res", average_img);
		cvCopy(previous_img, gray_img, NULL);
		int key = cvWaitKey(2);
		if (key == 27 || key == 10)
			break;

	}
//cvDrawContours(currentImg, self.currentcontours, (0, 0, 255), (0, 255, 0), 1, 2, cv.CV_FILLED);
}

int somethingIsMoving(void) {

	int no_black_pixles = 0;
	int i = 0;
	int j = 0;
	for (; i < i_height; i++) {
		for (; j < i_width; j++) {
			//printf("Bp = %f\n", CV_MAT_ELEM( *average_img, float, i, j ));
			if ((CV_MAT_ELEM( *average_img, double, i, j )) == 0.0)
				//printf("Bp=== = %d\n", no_black_pixles);
				no_black_pixles += 40;
		}
	}
//printf("Bp=== = %d\n", no_black_pixles);
	int averge_black_pixels = (no_black_pixles * 1000) / surface;
//printf("Black = %d\n", averge_black_pixels);
	if (averge_black_pixels > threshold)
		return 1;
	else
		return 0;
}
void processImage(IplImage* image) {

	cvCvtColor(image, previous_img, CV_RGB2GRAY);
	cvAbsDiff(gray_img, previous_img, average_img);
	cvSmooth(average_img, average_img, CV_BLUR, 5, 5, 0, 0);
	cvMorphologyEx(average_img, average_img, NULL, NULL, CV_MOP_OPEN, 1);
	cvMorphologyEx(average_img, average_img, NULL, NULL, CV_MOP_CLOSE, 1);
	cvThreshold(average_img, average_img, 10, 255, CV_THRESH_BINARY_INV);
}

int onChange() {
	return 5;
}

