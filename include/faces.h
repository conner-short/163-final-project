/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * faces.h: Drawing operations for face lists and faces
**/
#ifndef _FACES_H
#define _FACES_H

#include "vector.h"

struct face {
	unsigned int num_vertices;
	unsigned int* vertices_ccw;
	unsigned int num_triangles;
	unsigned int* triangles;
};

struct face_list {
	unsigned int num_faces;
	struct face* faces;
	unsigned int num_vertices;
	struct vector_3d* vertices;
};

void draw_face_list(struct face_list* list,
	struct vector_3d* viewer,
	struct vector_3d* light);

#endif
