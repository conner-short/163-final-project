/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * main.c: Main program
**/
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "camera.h"
#include "sim.h"
#include "view.h"

#define WINDOW_TITLE "163 Project"

void display() {
	draw_view();
}

void idle() {
	if(sim_main()) {glutPostRedisplay();}
	usleep(1000);
}

double x_translation;
double y_translation;
double z_translation;

double init_angle = 0.0;
double ampl = 0.5;
double angle = 0.0;

void move_cursor() {
	x_translation = ampl * cos(angle + init_angle);
	y_translation = 0.5 * x_translation;
	z_translation = ampl * sin(angle + init_angle);

	angle += M_PI / 75.0;
	if(angle >= 2.0 * M_PI) angle -= 2.0 * M_PI;
}

int main(int argc, char** argv) {
	if(!init_camera()) {
		fprintf(stderr, "Error: No camera detected. Exiting...\n");
		return 1;
	}

	glutInit(&argc, argv);
	glutInitWindowSize(900, 900);
	glutCreateWindow(WINDOW_TITLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	init_sim();
	init_view();

	sim_register_periodic_function(capture_frame, 30.0, 0);
	sim_register_periodic_function(move_cursor, 60.0, 1);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
