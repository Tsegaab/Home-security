#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>

int main(int argc, char** argv) {
	CvCapture* capture = cvCaptureFromCAM(0); // capture from video device #0
	//CvCapture* capture = cvCaptureFromAVI("infile.avi");
	//IplImage* img = 0;

	if (!cvGrabFrame(capture)) {              // capture a frame
		printf("Could not grab a frame\n\7");
		exit(0);
	}
	//img = cvRetrieveFrame(capture);
	CvVideoWriter *writer = 0;
	int isColor = 1;
	int fps = 25;  // or 30
	int frameW = 640; // 744 for firewire cameras
	int frameH = 480; // 480 for firewire cameras
	time_t today_time = time(NULL);
	struct tm * timeinfo;
	char *s=ctime(&today_time);
	s[strlen(s)-14]=s[strlen(s)-5];
	s[strlen(s)-13]=s[strlen(s)-4];
	s[strlen(s)-12]=s[strlen(s)-3];
	s[strlen(s)-11]=s[strlen(s)-2];
	s[strlen(s)-10]='.';
	s[strlen(s)-9]='a';
	s[strlen(s)-8]='v';
	s[strlen(s)-7]='i';
	s[strlen(s)-3]=0;
	s[strlen(s)-2]=0;
	s[strlen(s)-1]=0;
	printf ( "Current local time and date: %s. and", s );
	writer = cvCreateVideoWriter(s, CV_FOURCC('P', 'I', 'M', '1'), fps,
			cvSize(frameW, frameH), isColor);
	IplImage* img = 0;
	int nFrames = 950;
	int i = 0;
	for (i = 0; i < nFrames; i++) {
		cvGrabFrame(capture);          // capture a frame
		img = cvQueryFrame(capture);  // retrieve the captured frame
		cvWriteFrame(writer, img);      // add the frame to the file
	}
}
