/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 *  camera.cpp: OpenCV camera functionality and frame-fetch methods
**/
#include <opencv2/opencv.hpp>
#include <iostream>

#include "camera.h"

using namespace cv;

Mat* frame;
VideoCapture* camera;

unsigned char* frame_data;
int frame_cols;
int frame_rows;

int init_camera() {
	frame = new Mat;

	camera = new VideoCapture(0);
	if(!camera->isOpened()) {return 0;}

	return 1;
}

void capture_frame() {
	*camera >> *frame;

	frame_cols = frame->cols;
	frame_rows = frame->rows;
	frame_data = frame->data;
}
