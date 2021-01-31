/*!
 * @file ece486_fir.h
 *
 * @brief FIR filter using fixed-length input sample blocks
 *
 * @author ECE486 Lab Group ????
 * @author Your Name
 *
 * @date ????
 *
 * @defgroup ece486_fir Function Interface Requirements for ECE 486 Lab 2 FIR Filters
 *   @{
 *
 * Implementation of FIR filters for in which a very long input sequence
 * x[n] can be processed by breaking the input sequence into fixed length blocks.
 *
 * Filters are initialized by a single call to init_fir(), which allocates
 * and initializes a filter descriptor structure of type FIR_T
 *
 * @code
 *    #include "ece486_fir.h"
 *    ...
 *    FIR_T  *filt;
 *    ...
 *    // Define an impulse response vector h of length M...
 *    // Allocate memory for inputs x[i] and outputs y[i] of length "blocksize"...
 *    ...
 *    filt = init_fir(h,M,blocksize);
 * @endcode
 *
 * Once a filter descriptor is initialized, repeated calls to calc_fir()
 * provide the filter outputs, where the input samples are provided in fixed
 * length blocks of "blocksize" samples at a time.
 *
 * @code
 *    while( ...input sample blocks are available...) {
 *       // fill the vector x[i] with the next "blocksize" samples from the input sequence.
 *       calc_fir(filt, x, y);
 *       // do something with the filter output samples y[i].
 *    }
 * @endcode
 *
 * When processing completes, destroy_fir() should be called to release
 * any allocated memory.
 *
 * @code
 *    destroy_fir(filt);
 * @endcode
 *
 * @note
 * All input and output vectors (x[i] and y[i]) must be allocated by the caller.
 *
 * @note
 * The input and output arrays MAY point to the same location, in which case the
 * input sequence x[i] is replaced by the output
 * sequence y[i] in the calls to calc_fir().
 *
 */


#ifndef ECE486_FIR
#define ECE486_FIR

/*
 * Parameter Structure Definitions
 */

/*!
 * @brief FIR filter structure containing coefficients and filter state
 *
 * ECE486 Students will need to modify the contents of this structure for lab 2
 */
typedef struct your_fir_struct {
  float *fir_coefs;
  int n_coef;
  int blocksize;
  float *buffer;
  int Index;
          //  Actual contents will depend on implementation
    } FIR_T;

/*
 * Function Prototypes
 */

/*!
 * @brief FIR filter initialization
 *
 * Call init_fir() to create and initialize an FIR filter structure.  The
 * input array @a fir_coefs[i] should contain the ith sample of the impulse
 * response of the filter: h(i), i=0,1,...,n_coef-1.
 *
 * @returns pointer to an initialized structure of type #FIR_T which may
 * be used to calculate the requested filter outputs.
 */
FIR_T *init_fir(
  float *fir_coefs,     //!< [in] Pointer to filter coefficient array
  int   n_coef,         //!< [in] Total number of filter coefficients
  int   blocksize	//!< [in] Block size for input sample blocks
);

/*!
 * @brief Calculate a block of output samples of an FIR filter from an input sample block
 *
 * Calling calc_fir() repeatedly generates the filter output for an arbitrary sequence
 * of input samples.  The FIR filter output samples y(n) are given by
 * @verbatim
 *
 *                    i=n_coef-1
 *              y(n) =    sum     h(i) x(n-i)
 *                        i=0
 * @endverbatim
 * For each call, an input block of samples @a x[i] with length "blocksize" (as specified
 * in init_fir() ) is processed to generate the corresponding output samples
 * in the output vector @a y[i]. In subsequent calls to calc_fir(), the
 * new input blocks @a x[i] are assumed to follow the input blocks provided in
 * the previous calls to calc_fir().  As a result, very long input sequences
 * may be processed by breaking the sequence into many fixed-length blocks of
 * "blocksize" samples.
 *
 * Memory required to store the output vector @a y[i] must be allocated by the
 * caller.
 *
 * \note
 *   @a x and @a y CAN point to the same location.  In this case, the filter output
 *   samples will replace the input sample array.
 *
 * @returns On return, y[i] contains the calculated output samples.
 *  (The contents of the structure @a *s are also modified to keep track of the filter state.)
 *
 * @sa init_fir()
 */
void calc_fir(
  FIR_T *s,      	//!< [in,out] Pointer to previously initialized FIR filter structure,
                        //!< as provided by init_fir() (and possibly modified by
		        //!< previous calls to calc_fir() )
  float *x,             //!< [in] pointer to the input sample array
  float *y              //!< [out] Pointer to an array for storage of output samples
);

/*!
 * @brief Release memory associated with an #FIR_T
 */
void destroy_fir(
  FIR_T *s		//!< [in] Pointer to previously initialized FIR filter
			//!< structure, provided by init_fir()
);

/*! @} end of ece486_fir group. */
#endif
