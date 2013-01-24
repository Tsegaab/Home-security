/*
 * s_main.c
 *
 *  Created on: Jan 24, 2013
 *      Author: tekes
 *      Copyright (c) 2009 Adii Corporation \n All Right Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include "detect.h"

int main(int argc, char** argv) {
	if( !initDetector() )
		exit(0);
	runDetection();
}



