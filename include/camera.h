/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * camera.h: Camera module interface
**/
#ifndef _CAMERA_H
#define _CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char* frame_data;
extern int frame_cols;
extern int frame_rows;

/**
 * Initializes the camera and makes an OpenCV matrix to hold captured frame data
 *
 * Returns 1 on success, 0 on failure
**/
int init_camera();

/**
 * Captures a single frame from the camera and updates the frame_data,
 * frame_cols, and frame_rows
**/
void capture_frame();

#ifdef __cplusplus
}
#endif
#endif
