/**
 * opencv: Camera/OpenGL integration demo
 *
 * Conner Short
 * CMPE 163
 * November 2017
 *
 * sim.c: Simulation control functions
**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sim.h"

struct timed_func_node {
	void (*function_ptr)();
	double tick_rate;
	double interp_factor;
	int persistent;
	struct timespec l_tick;
	struct timed_func_node* next;
};

struct timed_func_node* root = NULL;
struct timed_func_node* last = NULL;

int num_timed_functions = 0;

void end_sim() {
	struct timed_func_node* i = root;
	struct timed_func_node* j;

	while(i != NULL) {
		j = i;
		i = i->next;
		free(j);
	}
}

void init_sim() {
	atexit(end_sim);
}

double diff_timespec(struct timespec* t0, struct timespec* t1) {
	double d_t0 = (double)t0->tv_sec + (double)t0->tv_nsec / 1000000000.0;
	double d_t1 = (double)t1->tv_sec + (double)t1->tv_nsec / 1000000000.0;
	return d_t1 - d_t0;
}

int sim_main() {
	int view_needs_updating = 0;
	struct timed_func_node* i = root;

	struct timespec c_time;
	clock_gettime(CLOCK_MONOTONIC, &c_time);

	while(i != NULL) {
		double d = diff_timespec(&(i->l_tick), &c_time);

		if(d > (1.0 / i->tick_rate)) {
			i->l_tick.tv_sec  = c_time.tv_sec;
			i->l_tick.tv_nsec = c_time.tv_nsec;

			if(i->persistent) {
				/* If persistence is desired, step the function until it is caught up
				 * with the elapsed time since the last run. */
				while(d > (1.0 / i->tick_rate)) {
					d -= (1.0 / i->tick_rate);
					i->function_ptr();
				}

				i->interp_factor = d * i->tick_rate;
			} else {
				/* Otherwise, just call it once. */
				i->function_ptr();
			}

			view_needs_updating = 1;
		}

		i = i->next;
	}

	return view_needs_updating;
}

int sim_register_periodic_function(void (*callback)(),
	double tick_rate,
	int persistent) {
		struct timed_func_node* n = malloc(sizeof(struct timed_func_node));

		if(n == NULL) {
			perror("sim_register_periodic_function");
			exit(EXIT_FAILURE);
		}

		clock_gettime(CLOCK_MONOTONIC, &(n->l_tick));

		n->function_ptr = callback;
		n->tick_rate = tick_rate;
		n->persistent = persistent;
		n->next = NULL;

		if(root == NULL) {
			root = n;
			last = n;
		} else {
			last->next = n;
			last = n;
		}

		num_timed_functions++;

		return num_timed_functions - 1;
}

double get_interp_factor(int func_id) {
	struct timed_func_node* i = root;

	while(i != NULL && func_id > 0) {
		i = i->next;
		func_id--;
	}

	return (i == NULL) ? 0.0 : i->interp_factor;
}
