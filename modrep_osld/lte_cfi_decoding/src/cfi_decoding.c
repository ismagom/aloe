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
#include <oesr.h>
#include <params.h>
#include <skeleton.h>

#define INCLUDE_DEFS_ONLY
#include "cfi_decoding.h"


/**
 * @ingroup lte_cfi_decoding
 * Sets the coding table
 * This table contains four 32-bit sequences, one for each CFI plus one,
 * the last, being reserved
 * @param table Coding table, 4x32 bits
 *
 */
inline void coding_table(char (*table)[NOF_BITS]) {

	int i, j;

	/* Initialize first 3 seqeunces with all ones */
	for (i=0;i<3;i++) {
		for (j=0;j<NOF_BITS;j++) {
			table[i][j] = 0x1;
		}
	}
	/* Now set to zero the zero-bits */
	for (j=0;j<11;j++) {	/* 11 zeros @ bit position 0, 3, 6, ..., 30*/
		table[0][3*j] = 0x0;
	}
	for (j=0;j<11;j++) {	/* 11 zeros @ bit position 1, 4, 7, ..., 31*/
		table[1][3*j+1] = 0x0;
	}
	for (j=0;j<10;j++) {	/* 10 zeros @ bit position 2, 5, 8, ..., 29*/
		table[2][3*j+2] = 0x0;
	}
	/* 4th sequence--all zeros--is reserved */
	for (j=0;j<NOF_BITS;j++) {
		table[3][j] = 0x0;
	}
}

/**
 * @ingroup lte_cfi_decoding
 * Decoding function
 * Decodes the recevied bitsequence to obtain the most likely CFI
 * The function performs bitwise XOR with each ot the coding sequences and
 * sums up the output. The minimum results indicates the code index and CFI.
 *
 * @param in Input bit sequence, received from descrambler, 32 bits
 * @param table Coding table, 4x32 bits
 * \returns Coding table index most similar to the recevied input sequence
 */
inline int cfi_decoding(char *in, char (*table)[NOF_BITS]) {

	int i, j;
	int count_i, index;
	int min = 32;

	for (i=0;i<4;i++) {
		count_i = 0;
		for (j=0;j<NOF_BITS;j++) {
			if (in[j] != table[i][j]) {
				count_i++;
			}
		}
		if (count_i < min) {
			min = count_i;
			index = i;
		}
		//printf("\nloop index %d: min = %d, index = %d", i, min, index);
	}
	return index;
}
