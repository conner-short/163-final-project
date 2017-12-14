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

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
