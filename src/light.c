/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * light.c: Lighting calculation system
**/
#include <math.h>
#include <stdint.h>

#include "vector.h"

/**
 * Performs lighting calculation for an input vertex and point light
 *
 * Parameters:
 *   l: Position of point light source
 *   n: Normal vector to vertex
 *   i: Input vertex
 *   k: Color coefficients
 *   o: Output color
**/
void light_vertex(struct vector_3d* l,
	struct vector_3d* n,
	struct vector_3d* i,
	struct color_rgb* k,
	struct color_rgb* o) {
	struct vector_3d r;
	
	/* Ray is defined pointing away from surface, like normal vector */
	/* Distances are scaled down to make darks visible */
	r.x = (l->x - i->x) / 360.0;
	r.y = (l->y - i->y) / 360.0;
	r.z = (l->z - i->z) / 360.0;
	
	double n_len = sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
	double r_len = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	
	double intensity = vector_3d_dot_product(n, &r)
		/ (n_len * r_len * r_len * r_len);
	
	if(intensity > 1.0) {intensity = 1.0;}
	else if(intensity < 0.0) {intensity = 0.0;}
	
	double r_tmp = (double)k->r * intensity;
	double g_tmp = (double)k->g * intensity;
	double b_tmp = (double)k->b * intensity;

	o->r = (uint8_t)r_tmp;
	o->g = (uint8_t)g_tmp;
	o->b = (uint8_t)b_tmp;
}
