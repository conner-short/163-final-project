/**
 * opencv: Camera/OpenGL integration demo
 *
 * Conner Short
 * CMPE 163
 * November 2017
 *
 * sim.h: Simulation control functions
**/
#ifndef _SIM_H
#define _SIM_H

/**
 * Initializes the simulation
**/
void init_sim();

/**
 * Runs the simulation, triggering any registered functions which have elapsed
 *
 * Returns a flag indicating if any of the functions were triggered
**/
int sim_main();

/**
 * Registers a function to be run periodically by the simulation
 *
 * Parameters:
 *   callback: Pointer to function to invoke upon completion
 *   tick_rate: Frequency in Hz to run the function
 *   persistent: If non-zero, the simulation will invoke the callback as many
 *     times as needed to "catch up" the function on any missed invocations.
 *     Otherwise, the simulation will run the function only once if it is due to
 *     be run
 *
 * Returns an index for the registered function
**/
int sim_register_peridoc_function(void (*callback)(),
	double tick_rate,
	int persistent);

/**
 * Retrieves an interpolation factor between zero and one specifying the amount
 * of unsimulated time following the last function invocation. Only valid for
 * persistent functions.
 *
 * Parameters:
 *   func_id: ID of the function to get a factor for
 *
 * Returns the interpolation factor if the function is persistent and 0.0 if the
 *   function is not persistent
**/
double get_interp_factor(int func_id);

#endif
