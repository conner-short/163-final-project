/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * light.h: Lighting calculation system
**/
#ifndef _LIGHT_H
#define _LIGHT_H

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
	struct color_rgb* o);

#endif
