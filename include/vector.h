/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * vertex.h: Vertex definitions and operations
**/
#ifndef _VERTEX_H
#define _VERTEX_H

struct vector_2d {double x, y;};
struct vector_3d {double x, y, z;};
struct color_rgb {double r, g, b;};

/**
 * Computes the dot product a ⋅ b
 *
 * Parameters:
 *   a, b: Input vectors
 *
 * Returns the result of the dot product
**/
double vector_3d_dot_product(struct vector_3d* a, struct vector_3d* b);

/**
 * Computes the cross product a × b
 *
 * Parameters:
 *   a, b: Input vectors
 *   o: The output vector
**/
void vector_3d_cross_product(struct vector_3d* a,
	struct vector_3d* b,
	struct vector_3d* o);

/**
 * Transforms a world-space point to a viewer-space point
 *
 * Parameters:
 *   v: Viewer coordinates in world space
 *   i: Input coordinates in world space
 *   o: Ouput coordinates in viewer space
**/
void wv_transform(struct vector_3d* v,
	struct vector_3d* i,
	struct vector_3d* o);

/**
 * Performs a perspective projection of a viewer-space point
 *
 * Parameters:
 *   i: Input coordinates in viewer space
 *   o: Transformed output coordinates
**/
void perspective_projection(struct vector_3d* i, struct vector_3d* o);

/**
 * Projects a ray into a plane
 *
 * Parameters:
 *   i: Input point through which ray is projected
 *   s: Source point of ray
 *   o: Output point at intersection of ray line and plane
 *   p1, p2, p3: Arbitrary points within target plane
**/
void ray_projection(struct vector_3d* i,
	struct vector_3d* s,
	struct vector_3d* o,
	struct vector_3d* p1,
	struct vector_3d* p2,
	struct vector_3d* p3);

#endif
