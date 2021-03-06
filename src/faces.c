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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "faces.h"
#include "light.h"
#include "vector.h"

void determine_face_normal(struct face* f,
	struct vector_3d* vertices,
	struct vector_3d* o) {
	struct vector_3d p1, p2;

	p1.x = vertices[f->vertices_ccw[1]].x - vertices[f->vertices_ccw[0]].x;
	p1.y = vertices[f->vertices_ccw[1]].y - vertices[f->vertices_ccw[0]].y;
	p1.z = vertices[f->vertices_ccw[1]].z - vertices[f->vertices_ccw[0]].z;

	p2.x = vertices[f->vertices_ccw[2]].x - vertices[f->vertices_ccw[0]].x;
	p2.y = vertices[f->vertices_ccw[2]].y - vertices[f->vertices_ccw[0]].y;
	p2.z = vertices[f->vertices_ccw[2]].z - vertices[f->vertices_ccw[0]].z;

	vector_3d_cross_product(&p1, &p2, o);

	double mag = sqrt(o->x * o->x + o->y * o->y + o->z * o->z);

	o->x /= mag;
	o->y /= mag;
	o->z /= mag;
}

void draw_face(struct face_list* l,
	struct face* f,
	struct vector_3d* light,
	struct vector_3d* viewer) {
	struct vector_3d* v_tmp = malloc(l->num_vertices * sizeof(struct vector_3d));
	struct color_rgb* colors = malloc(l->num_vertices * sizeof(struct color_rgb));

	struct vector_3d n;
	determine_face_normal(f, l->vertices, &n);

	struct color_rgb k = {0xCC, 0x00, 0x00};

	int i;
	for(i = 0; i < f->num_vertices; i++) {
		/* Calculate color for each face vertex */
		light_vertex(light,
			&n,
			&(l->vertices[f->vertices_ccw[i]]),
			&k,
			&(colors[f->vertices_ccw[i]]));

		/* Transform each face vertex */
		wv_transform(viewer,
			&(l->vertices[f->vertices_ccw[i]]),
			&(v_tmp[f->vertices_ccw[i]]));
		perspective_projection(&(v_tmp[f->vertices_ccw[i]]),
			&(v_tmp[f->vertices_ccw[i]]));
	}

	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_DOUBLE, 0, v_tmp);
	glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);

	glDrawElements(GL_TRIANGLES,
		3 * f->num_triangles,
		GL_UNSIGNED_INT,
		f->triangles);

	glDisableClientState(GL_COLOR_ARRAY);

	free(v_tmp);
	free(colors);
}

void draw_face_shadow(struct face* f,
	struct vector_3d* vertices,
	struct vector_3d* viewer,
	struct vector_3d* light) {
	struct vector_3d v_tmp[3];

	struct vector_3d p1 = {.x = 0.0, .y = 0.0, .z = -1.0};
  struct vector_3d p2 = {.x = 1.0, .y = 0.0, .z = -1.0};
  struct vector_3d p3 = {.x = 0.0, .y = 1.0, .z = -1.0};

	/* Draw shadow triangles */

	int i;
	for(i = 0; i < 3 * f->num_triangles; i += 3) {
		/* Transform */
		ray_projection(&(vertices[f->triangles[i + 0]]),
			light,
			&(v_tmp[0]),
			&p1,
			&p2,
			&p3);
		wv_transform(viewer, &(v_tmp[0]), &(v_tmp[0]));
		perspective_projection(&(v_tmp[0]), &(v_tmp[0]));

		ray_projection(&(vertices[f->triangles[i + 1]]),
			light,
			&(v_tmp[1]),
			&p1,
			&p2,
			&p3);
		wv_transform(viewer, &(v_tmp[1]), &(v_tmp[1]));
		perspective_projection(&(v_tmp[1]), &(v_tmp[1]));

		ray_projection(&(vertices[f->triangles[i + 2]]),
			light,
			&(v_tmp[2]),
			&p1,
			&p2,
			&p3);
		wv_transform(viewer, &(v_tmp[2]), &(v_tmp[2]));
		perspective_projection(&(v_tmp[2]), &(v_tmp[2]));

		/* Draw */
		glColor3d(0.0, 0.0, 0.0);
		glVertexPointer(3, GL_DOUBLE, 0, v_tmp);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

void draw_face_list_shadow(struct face_list* list,
	struct vector_3d* viewer,
	struct vector_3d* light) {
	int i;
	for(i = 0; i < list->num_faces; i++) {
		struct vector_3d n;
		determine_face_normal(&(list->faces[i]), list->vertices, &n);

		/* Calculate signed distance used to determine if the plane is in light
		 * or not */
		double signed_lp_dist = vector_3d_dot_product(&n, light);
		double p_dist = vector_3d_dot_product(
			&(list->vertices[list->faces[i].vertices_ccw[0]]), 
			&n);

		signed_lp_dist += p_dist;

		if(signed_lp_dist > 0.0) {
			/* Plane is in light, so it contributes to the silhouette. Draw its
			 * shadow */
			draw_face_shadow(&(list->faces[i]), list->vertices, viewer, light);
		}
	}
}

void draw_face_list(struct face_list* list,
	struct vector_3d* viewer,
	struct vector_3d* light) {
	draw_face_list_shadow(list, viewer, light);

	int i;
	for(i = 0; i < list->num_faces; i++) {
		draw_face(list, &(list->faces[i]), light, viewer);
	}
}
