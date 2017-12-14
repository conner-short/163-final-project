/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * faces.c: Drawing operations for face lists and faces
**/
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include "faces.h"
#include "vector.h"

struct vector_3d* determine_face_normal(struct face* f,
	struct vector_3d* vertices) {
	if(f->num_vertices < 3) {return NULL;}

	struct vector_3d* n = malloc(sizeof(struct vector_3d));
	if(n == NULL) {
		perror("determine_face_normal");
		exit(EXIT_FAILURE);
	}

	struct vector_3d p1, p2;

	p1.x = vertices[f->vertices_ccw[1]].x - vertices[f->vertices_ccw[0]].x;
	p1.y = vertices[f->vertices_ccw[1]].y - vertices[f->vertices_ccw[0]].y;
	p1.z = vertices[f->vertices_ccw[1]].z - vertices[f->vertices_ccw[0]].z;

	p2.x = vertices[f->vertices_ccw[2]].x - vertices[f->vertices_ccw[0]].x;
	p2.y = vertices[f->vertices_ccw[2]].y - vertices[f->vertices_ccw[0]].y;
	p2.z = vertices[f->vertices_ccw[2]].z - vertices[f->vertices_ccw[0]].z;

	vector_3d_cross_product(&p1, &p2, n);

	return n;
}

void draw_face(struct face* f,
	struct vector_3d* vertices,
	struct vector_3d* viewer) {
	glColor3d(0.8, 0.0, 0.0);
	glVertexPointer(3, GL_DOUBLE, 0, vertices);

	glDrawElements(GL_TRIANGLES,
		3 * f->num_triangles,
		GL_UNSIGNED_INT,
		f->triangles);
}

void draw_face_shadow(struct face* f, struct vector_3d* n) {
}

void draw_face_list_shadow(struct face_list* l) {
}

void draw_face_list(struct face_list* list,
	struct vector_3d* viewer,
	struct vector_3d* light) {
	struct vector_3d* v_tmp = malloc(list->num_vertices *
		sizeof(struct vector_3d));
	if(v_tmp == NULL) {
		perror("draw_face_list");
		exit(EXIT_FAILURE);
	}

	/*draw_face_list_shadow();*/

	/* Transform vertices */

	int i;
	for(i = 0; i < list->num_vertices; i++) {
		wv_transform(viewer, &(list->vertices[i]), &(v_tmp[i]));
		perspective_projection(&(v_tmp[i]), &(v_tmp[i]));
	}

	for(i = 0; i < list->num_faces; i++) {
		draw_face(&(list->faces[i]), v_tmp, viewer);
	}

	free(v_tmp);
}
