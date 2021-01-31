/*
*@file
*
*@brief ECE 486 IRR header file
*
*@author ECE486 Group 1
*@author John Bowen, Jeremy Juybari
*
*@date Febuary 2020
*
*This file contains the subroutine and data declarations for
*the running biquad filter calculations, in the form of
*Transposed Direct Form II.
*
*The BIQUAD_T data type is a struct with the necessary fields for
*calculating the transfer function.
*
*The init_biquad() function initializes a BIQUAD_T struct, and allocates memory.
*The function takes an integer 'sections' which is the number of biquad filter
*sections for the calculation of the transfer function. The float 'g' is passed as the gain to be
*multiplied by every biquad section. An integer 'blocksize' indicates the
*number of samples in each block. Float 'biquad_coefs' is a floating pointer to a
*two dimential array with 'v1' coeffinceinets and 'v2' coeffincients.
* the function returns a pointer to the BIQUAD_T struct.
*
*The calc_biquad() function performs the transfer function calcutions on each
*block of samples. It takes a pointer float 'x' (the block of samples) and
*a pointer to the BIQUAD_T 's'. 's' is the struct containing the fields necessary
*for the calcutions. the function returns 'y' which is a pointer to
*an array of floats. This array contains the output transfer function calculations.
*
*The destroy_biquad() function de-allocates the memory allocated in the init_biquad()
*and calc_biquad() fnctions.
*
*/

#ifndef ECE486_BIQUAD_H
#define ECE486_BIQUAD_H

/*!
* @brief Sturcture for IIR implemntaion. Contains coeffincients and filter state.
*
*/
typedef struct biquad{
/*!<Number of sections*/
	int sections;
/*!<The gain factor */
	float g;
/*!<The coeffincients*/
	float *biquad_coefs;
/*!<The number of samples*/
	int blocksize;
/*!<array of coeffincients*/
	float *v1;
/*!<array of coeffincients*/
	float *v2;

} BIQUAD_T;
/*!
*@brief Initializes a BIQUAD_T structure.
*
*@returns a pointer to the structure of type BIQUAD_T, containg the
*varaibles for the IIR filter implemntaion.
*/

BIQUAD_T *init_biquad(
/*!<Number of sections*/
	int sections,
/*!<The gain factor */
	float g,
/*!<The coeffincients*/
	float *biquad_coefs,
	/*!<The number of samples*/
	int blocksize
);
/*!
*@brief Does the IRR file calcutions for each block
*
*returns the output array @y from the input sample array @x, which
*is calculated using the filter infromation stored in @s.
*/
void calc_biquad(
/*!<Structure contain the IIR filter function stuff>*/
	BIQUAD_T *s,
/*!<Array of inputs>*/
	float *x,
/*!<Array of outputs>*/
	float *y
);
/*!
*@brief De-allocates the memory of the BIQUAD_T struct.
*
*/
void destroy_biquad(
/*!<Pointer to a BIQUAD_T struct to be cleared */
	BIQUAD_T *s
);

#endif // ECE486_BIQUAD_H
