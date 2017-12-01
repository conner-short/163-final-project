/**
 * project: 163 final project
 *
 * Conner Short
 * CMPE 163
 * December 2017
 *
 * vector.c: Vector transformation procedures
**/
#include <math.h>

#include "vector.h"

const double p_plane_dist = 1.4;

/**
 * Computes the dot product a ⋅ b
 *
 * Parameters:
 *   a, b: Input vectors
 *
 * Returns the result of the dot product
**/
double vector_3d_dot_product(struct vector_3d* a, struct vector_3d* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

/**
 * Computes the cross product a × b
 *
 * Parameters:
 *   a, b: Input vectors
 *   o: The output vector
**/
void vector_3d_cross_product(struct vector_3d* a,
	struct vector_3d* b,
	struct vector_3d* o) {
	struct vector_3d _o;
  
	_o.x = a->y * b->z - a->z * b->y;
	_o.y = a->z * b->x - a->x * b->z;
	_o.z = a->x * b->y - a->y * b->x;
  
	o->x = _o.x;
	o->y = _o.y;
	o->z = _o.z;
}

void wv_transform(struct vector_3d* v,
	struct vector_3d* i,
	struct vector_3d* o) {
	struct vector_3d _o;

	/* Calculate hypotenuses */
	double v_hyp = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	double xy_hyp = sqrt(v->x * v->x + v->y * v->y);

	/* Calculate sines and cosines */
	double sin_phi   = xy_hyp / v_hyp;
  double cos_phi   = v->z / v_hyp;
	double sin_theta = v->y / xy_hyp;
	double cos_theta = v->x / xy_hyp;

	_o.x = i->x * (-sin_theta) + i->y * (cos_theta);
	_o.y = i->x * (-cos_phi * cos_theta) + i->y * (-cos_phi * sin_theta)
  	+ i->z * (sin_phi);
	_o.z = i->x * (-sin_phi * cos_theta) + i->y * (-sin_phi * sin_theta)
  	+ i->z * (-cos_phi) + v_hyp;

	o->x = _o.x;
	o->y = _o.y;
	o->z = _o.z;
}

void perspective_projection(struct vector_3d* i, struct vector_3d* o) {
	o->x = i->x * p_plane_dist / i->z;
	o->y = i->y * p_plane_dist / i->z;
	o->z = 0.0;
}

void ray_projection(struct vector_3d* i,
	struct vector_3d* s,
	struct vector_3d* o,
	struct vector_3d* p1,
	struct vector_3d* p2,
	struct vector_3d* p3) {
		struct vector_3d ray, s_p1, normal, _normal_a, _normal_b;

		/* Calculate the ray and normalize it to find the direction of the ray */

		ray.x = i->x - s->x;
		ray.y = i->y - s->y;
		ray.z = i->z - s->z;

		/* Use the plane points to calculate the normal vector */

		_normal_a.x = p2->x - p1->x;
		_normal_a.y = p2->y - p1->y;
		_normal_a.z = p2->z - p1->z;

		_normal_b.x = p3->x - p1->x;
		_normal_b.y = p3->y - p1->y;
		_normal_b.z = p3->z - p1->z;

		vector_3d_cross_product(&_normal_a, &_normal_b, &normal);

		/* Calculate s - p1 */
		s_p1.x = s->x - p1->x;
		s_p1.y = s->y - p1->y;
		s_p1.z = s->z - p1->z;

		/* Calculate the scalar to multiply by the ray direction vector */
		double r = vector_3d_dot_product(&normal, &s_p1);
		r /= vector_3d_dot_product(&normal, &ray);

		o->x = s->x - r * ray.x;
		o->y = s->y - r * ray.y;
		o->z = s->z - r * ray.z;
}
