/*
 * detect.h
 *
 *  Created on: Jan 24, 2013
 *      Author: tekes
 *      Copyright (c) 2009 Adii Corporation \n All Right Reserved.
 */
#include "cv.h"
#include "highgui.h"

#ifndef DETECT_H_
#define DETECT_H_

int initDetector();
void processImage(IplImage* image);
int somethingIsMoving(void);
void runDetection();
int onChange();

#endif /* DETECT_H_ */
