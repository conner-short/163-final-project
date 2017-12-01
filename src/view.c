/**
 * opencv: Camera/OpenGL integration demo
 *
 * Conner Short
 * CMPE 163
 * November 2017
 *
 * view.c: Drawing routines and callbacks
**/
#include <GL/gl.h>
#include <GL/glut.h>

#include "camera.h"
#include "vector.h"
#include "view.h"

GLuint texture_name;

unsigned char* l_frame_data;
int l_frame_cols;
int l_frame_rows;

/**
 * Vertex definitions
**/

/* Cube */

struct vector_3d cube_vertices[8] = {
  {.x = -1.0, .y = -1.0, .z = -1.0},
  {.x =  1.0, .y = -1.0, .z = -1.0},
  {.x =  1.0, .y =  1.0, .z = -1.0},
  {.x = -1.0, .y =  1.0, .z = -1.0},
  {.x = -1.0, .y = -1.0, .z =  1.0},
  {.x =  1.0, .y = -1.0, .z =  1.0},
  {.x =  1.0, .y =  1.0, .z =  1.0},
  {.x = -1.0, .y =  1.0, .z =  1.0}
};

struct vector_3d t_cube_vertices[8];

unsigned int cube_lines[24] = {
	0, 1,    1, 2,    2, 3,    3, 0,
	4, 5,    5, 6,    6, 7,    7, 4,
	0, 4,    1, 5,    2, 6,    3, 7
};

unsigned int cube_floor_triangles[6] = {
	0, 1, 3,
	3, 1, 2
};

/* Arrow */

struct vector_3d arrow_vertices[7] = {
  {.x =  0.0000, .y =  0.0000, .z = 0.0},
  {.x =  0.1250, .y = -0.1250, .z = 0.0},
  {.x =  0.0625, .y = -0.1250, .z = 0.0},
  {.x =  0.0625, .y = -0.2500, .z = 0.0},
  {.x = -0.0625, .y = -0.2500, .z = 0.0},
  {.x = -0.0625, .y = -0.1250, .z = 0.0},
  {.x = -0.1250, .y = -0.1250, .z = 0.0}
};

unsigned int arrow_triangles[9] = {
  0, 1, 6,
  2, 3, 4,
  4, 5, 2
};

/* Misc */

struct vector_3d light  = {.x = -0.5, .y = 0.5, .z = 2.0};
struct vector_3d viewer = {.x = 0.0, .y = 3.0, .z = 2.0};

void init_view() {
	int i;
	
	/* Enable GL array support */
	glEnableClientState(GL_VERTEX_ARRAY);
	
	/* Perform cube coordinate transformation early since it won't change each
	 * frame; saves some time */
	for(i = 0; i < 8; i++) {
		wv_transform(&viewer, &(cube_vertices[i]), &(t_cube_vertices[i]));
		perspective_projection(&(t_cube_vertices[i]), &(t_cube_vertices[i]));
	}
}

void draw_cube() {
	glColor3d(1.0, 1.0, 1.0);
	
	glVertexPointer(3, GL_DOUBLE, 0, t_cube_vertices);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, cube_lines);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, cube_floor_triangles);
}

void draw_shaded_arrow() {
	struct vector_3d tmp_arrow_vertices[7];
	struct vector_3d tmp_shadow_vertices[7];
	int i;
	
	/* Calculate the shadow vertices */
	for(i = 0; i < 7; i++) {
		ray_projection(&(arrow_vertices[i]),
			&light,
			&(tmp_shadow_vertices[i]),
			&(cube_vertices[0]),
			&(cube_vertices[1]),
			&(cube_vertices[2]));
	}
	
	/* Transform */
	for(i = 0; i < 7; i++) {
		wv_transform(&viewer, &(arrow_vertices[i]), &(tmp_arrow_vertices[i]));
		perspective_projection(&(tmp_arrow_vertices[i]), &(tmp_arrow_vertices[i]));
		
		wv_transform(&viewer, &(tmp_shadow_vertices[i]), &(tmp_shadow_vertices[i]));
		perspective_projection(&(tmp_shadow_vertices[i]),
			&(tmp_shadow_vertices[i]));
	}
	
	/* Draw the shadow */
	glColor3d(0.0, 0.0, 0.0);
	
	glVertexPointer(3, GL_DOUBLE, 0, tmp_shadow_vertices);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, arrow_triangles);
	
	/* Draw the arrow */
	glColor3d(1.0, 0.0, 0.0);
	
	glVertexPointer(3, GL_DOUBLE, 0, tmp_arrow_vertices);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, arrow_triangles);
}

void draw_view() {
	glClear(GL_COLOR_BUFFER_BIT);

	draw_cube();
	draw_shaded_arrow();

	glutSwapBuffers();
}
