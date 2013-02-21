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


/* Maximum number of integers containing the scrambling sequence bits */
#define MAX_c			896 /* INPUT_MAX_SAMPLES/32 */
#define MAX_x			947 /* (INPUT_MAX_SAMPLES+Nc_default)/32 */

/* Scrambling sequence generation parameters */
#define Nc_default 		1600	/* fixed */
#define q_default		0
#define nrnti_default		0
#define cell_gr_default		0
#define cell_sec_default	0

/* scrambling sequence generation parameters */
struct scrambling_params {
	int q;			/* codeword index: 0 or 1*/
	int Nc;
	int cell_gr;		/* Cell ID group index */
	int cell_sec;		/* cell ID sector index */
	int nrnti;		/* Radio network temporary identifier */

};

/* Function prototypes */
void char2int(char *input, unsigned *output, int N);
void int2char(unsigned *input, char *output, int N, int rem_bits);
void sequence_generation(unsigned (*c)[10], struct scrambling_params params);
void compute_x1(void);
void compute_x2(unsigned *c_init);
