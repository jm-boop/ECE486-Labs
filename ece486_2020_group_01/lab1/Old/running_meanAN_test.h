#ifndef M_struct_H
#define M_struct_H

#include <stdint.h>


typedef struct M_struct {

	 	uint32_t M;
		uint32_t blocksize;
		float previous;
		float * last;
		uint32_t lastIndex;

} M_struct;

 //brief Initializes a structure.

 //returns A pointer to a structure of type M_struct is returned

M_struct * init_running_Mean(
	uint32_t M,
	uint32_t blocksize
);


//brief Calculates the mean value of a block of samples.


float * calc_runningMean(
	float * x,
	M_struct * s
);


 //brief De-allocates the memory


void destroy_msv(
	M_struct * s,
	float * y
);

#endif
