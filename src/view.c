/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * view.c: Drawing routines and callbacks
**/
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "vector.h"
#include "view.h"

GLuint texture_name;

unsigned char* vertex_color_data;

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

/* Plane */

struct vector_3d plane_vertices[4] = {
	{.x =  1.0, .y =  3.0, .z =  1.0},
	{.x = -1.5, .y =  1.0, .z =  1.0},
	{.x = -1.5, .y =  1.0, .z = -1.0},
	{.x =  1.0, .y =  3.0, .z = -1.0}
};

struct vector_3d* plane_texel_vertices = NULL;
struct color_rgb_uint8* plane_texel_colors = NULL;
int num_texel_vertices;

/* Misc */

struct vector_3d light  = {.x = -0.5, .y = 0.5, .z = 2.0};
struct vector_3d viewer = {.x = 0.0, .y = -2.4, .z = 1.6};

void end_view() {
	if(plane_texel_vertices != NULL) free(plane_texel_vertices);
	if(plane_texel_colors != NULL) free(plane_texel_colors);
}

void init_view() {
	atexit(end_view);

	int i, j;
	
	/* Enable GL array support */
	glEnableClientState(GL_VERTEX_ARRAY);
	
	/* Perform cube coordinate transformation early since it won't change each
	 * frame; saves some time */
	for(i = 0; i < 8; i++) {
		wv_transform(&viewer, &(cube_vertices[i]), &(t_cube_vertices[i]));
		perspective_projection(&(t_cube_vertices[i]), &(t_cube_vertices[i]));
	}

	/* Allocate space for texels */

	capture_frame();

	num_texel_vertices = (frame_cols + 1) * (2 * frame_rows);

	plane_texel_vertices = malloc(num_texel_vertices * sizeof(struct vector_3d));
	if(plane_texel_vertices == NULL) {
		perror("init_view");
		exit(EXIT_FAILURE);
	}

	plane_texel_colors = malloc(num_texel_vertices * 
		sizeof(struct color_rgb_uint8));
	if(plane_texel_colors == NULL) {
		perror("init_view");
		exit(EXIT_FAILURE);
	}

	/* Determine texel coordinates */

	double dx = plane_vertices[1].x - plane_vertices[0].x;
	double dy = plane_vertices[1].y - plane_vertices[0].y;

	double plane_width = sqrt(dx * dx + dy * dy);
	double z_init = 0.5 * plane_width * (double)frame_rows / (double)frame_cols;
	double dz = -2.0 * z_init;

	dx /= (double)frame_cols;
	dy /= (double)frame_cols;
	dz /= (double)frame_rows;

	for(i = 0; i <= frame_cols; i++) {
		double x = plane_vertices[0].x + dx * (double)i;
		double y = plane_vertices[0].y + dy * (double)i;

		for(j = 0; j < frame_rows; j++) {
			double z = z_init + dz * (double)j;

			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i].x = x;
			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i].y = y;
			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i].z = z;
			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i + 1].x = x;
			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i + 1].y = y;
			plane_texel_vertices[2 * j * (frame_cols + 1) + 2 * i + 1].z = z + dz;
		}
	}

	/* Transform texel coordinates */

	for(i = 0; i < num_texel_vertices; i++) {
		wv_transform(&viewer,
			&(plane_texel_vertices[i]),
			&(plane_texel_vertices[i]));
		perspective_projection(&(plane_texel_vertices[i]),
			&(plane_texel_vertices[i]));
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

void copy_colors() {
	int i, j;

	for(i = 0; i < frame_rows; i++) {
		int fd_index = 3 * i * frame_cols;
		int v_index = 2 * i * (frame_cols + 1);

		for(j = 0; j < frame_cols; j++) {
			int r = frame_data[fd_index + 2];
			int g = frame_data[fd_index + 1];
			int b = frame_data[fd_index + 0];

			plane_texel_colors[v_index + 0].r = r;
			plane_texel_colors[v_index + 0].g = g;
			plane_texel_colors[v_index + 0].b = b;

			plane_texel_colors[v_index + 1].r = r;
			plane_texel_colors[v_index + 1].g = g;
			plane_texel_colors[v_index + 1].b = b;

			fd_index += 3;
			v_index  += 2;
		}
	}
}

void draw_plane() {
	copy_colors();

	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_DOUBLE, 0, plane_texel_vertices);
	glColorPointer(3, GL_UNSIGNED_BYTE, 0, plane_texel_colors);

	int i;

	for(i = 0; i < frame_rows; i++) {
		glDrawArrays(GL_QUAD_STRIP, 2 * i * (frame_cols + 1), 2 * (frame_cols + 1));
	}

	glDisableClientState(GL_COLOR_ARRAY);
}

void draw_view() {
	glClear(GL_COLOR_BUFFER_BIT);

	draw_plane();
	draw_cube();
	draw_shaded_arrow();

	glutSwapBuffers();
}
