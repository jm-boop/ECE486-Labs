/*
 **@file
 *
 *@brief test of ece486_nco function and routines 
 *
 *@author ECE486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date April 2020.
 *
 *
 */

// TEST FILE FOR NCO
//
//
//
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ece486_nco.h"

int main(int argc, char *argv[]) {

	float f0 = 0.1;
	float theta = 0.0;
	float new_f = 0.0;
	FILE *fh = fopen("nco.txt", "w");
	if (fh == NULL) {
		printf("Failed to open file\n");
	}
	// Test initialization function
	NCO_T *s = init_nco(f0, theta);

	// Show details for constructed NCO
	printf("%f, %f\n", s->f0_step, s->theta);

	// Print out cosine values, IT WORKED!
/*	int i;
	for (i=0; i < 512; i++) {
		printf("%f\n", s->lookup[i]);
	}
*/

	// TEST NCO GENERATION
	if (argv[1] != NULL) {
		new_f = atof(argv[1]);
		nco_set_frequency(s, new_f);
	}
	int length = 512;
	float y[length];
	nco_get_samples(s, y, length);
	for (int i = 0; i < length; i++) {
		//printf("%f\n", y[i]);
		fprintf(fh, "%f\n", y[i]);
	}
	/*
	float y2[5];
	nco_set_phase(s, 0.2);
	nco_get_samples(s, y2, 5);
	for (int i = 0; i < 5; i++) {
		printf("%f\n", y2[i]);
	}
*/
	// Free memory
	destroy_nco(s);

	// Close the file
	if (fh != NULL) {
		fclose(fh);
	}

	return 0;
}
