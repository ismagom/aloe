/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include <aloe/swapi_static.h>
#include <aloe/skeleton.h>
#include <aloe/params.h>

#define MODULE_CONFIG
#include "test_tx.h"
#undef MODULE_CONFIG

int *block_length;
int cnt=0;

/*
 * Function documentation
 *
 * @returns 0 on success, -1 on error
 */
int initialize() {

	block_length = param_get_addr("block_length");
	if (!block_length) {
		moderror_msg("Error: %s\n",param_error_str());
		return 0;
	}

	modinfo_msg("Parameter block_length is %d\n",*block_length);

	/* Verify control parameters */
	if (*block_length > get_output_max_samples()) {
		moderror_msg("Invalid block length %d\n", *block_length);
		return -1;
	}


	/* do some other initialization stuff */

	return 0;
}



/**
 * @brief Function documentation
 *
 * @param inp Input interface buffers. Data from other interfaces is stacked in the buffer.
 * Use in(ptr,idx) to access the address. To obtain the number of received samples use the function
 * int get_input_samples(int idx) where idx is the interface index.
 *
 * @param out Input interface buffers. Data to other interfaces must be stacked in the buffer.
 * Use out(ptr,idx) to access the address.
 *
 * @return On success, returns a non-negative number indicating the output
 * samples that should be transmitted through all output interface. To specify a different length
 * for certain interface, use the function set_output_samples(int idx, int len)
 * On error returns -1.
 *
 *
 */
int work(void **inp, void **out) {
	int snd_samples = *block_length;
	int i;
	input_t *input = inp[0];
	output_t *output = out[0];

	/* do DSP stuff here */
	for (i=0;i<snd_samples;i++) {
		__real__ input[i]=i+cnt;
		__imag__ input[i]=snd_samples-i+cnt;
	}
	cnt++;
	return snd_samples;
}

/** @brief Deallocates resources created during initialize().
 * @return 0 on success -1 on error
 */
int stop() {
	return 0;
}

param_t *param_list(int *nof_params) {
	*nof_params = nof_parameters;
	return parameters;
}


