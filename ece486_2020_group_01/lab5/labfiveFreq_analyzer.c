/*
 **@file
 *
 *@brief ECE 486 Lab 5 - Build a frequency analyzer with programmable center frequency
 *
 *@author ECE 486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date April 2020
 *
 */

#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ece486_nco.h"
#include <stdint.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdint.h>

// To detect a button press
extern FlagStatus KeyPressed;

int main(void)
{
  	/*
   	* Set up ADCs, DACs, GPIO, Clocks, DMAs, and Timer
   	*
   	* If your discovery board has been modified to support the external
   	* HSE reference, use the (better quality crystal controlled) HSE
   	* configuration.  If not, you can use the MSI reference, and things
   	* should still work -- but your clocks will drift over time.
   	*
   	* The MSI (Medium speed internal) reference is the on-chip RC oscillator
   	*
   	* The HSE (High speed external) reference is an external clock, either
   	* provided through the external connector, or (if your board is modified)
   	* from the crystal reference source generated by the other processor
	* on the development board.
	*/

 	int nsamp; // Size of input and output blocks
	int i, j; // Iteration variables
	float *input, *output1, *output2; // Arrays for input and output blocks
	int counter = 0; // Track number of input blocks
	int button_count = 0; // Track number of button presses
  	int f0 = 0; // Center frequency for the mixer
	uint16_t numTaps = 66; // Number of coefficients for FIR filter
	uint8_t M = 10; // Decimation factor
	uint32_t blockSize; // Size of input and output blocks

	// Create ARM FIR filter instances
	arm_fir_decimate_instance_f32 s;
  	arm_fir_decimate_instance_f32 s3;
        
	// Set the blocksize to 1280 > fftsize=1024
	setblocksize(1280);

	//initialize_ece486(FS_50K, MONO_IN, STEREO_OUT, MSI_INTERNAL_RC);
  	initialize_ece486(FS_32K, MONO_IN, STEREO_OUT, HSE_EXTERNAL_8MHz);

  	nsamp = getblocksize(); // Keep track of blocksize
	blockSize = nsamp;
  	
	// Memory Allocation
  	input = (float *)malloc(sizeof(float)*nsamp);
  	output1 = (float *)malloc(sizeof(float)*nsamp);
  	output2 = (float *)malloc(sizeof(float)*nsamp);

	// Keep track of decimated samples
  	float32_t *decBlock1 = (float32_t *)calloc(nsamp/10, sizeof(float32_t));
  	float32_t *decBlock2 = (float32_t *)calloc(nsamp/10, sizeof(float32_t));
	
	// Keep track of FIR filter states between blocks
	float32_t *pState = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));
  	float32_t *pState2 = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));
	
	// Check for memory allocation errors
  	if (input==NULL||output1==NULL||output2==NULL) {
    		flagerror(MEMORY_ALLOCATION_ERROR);
    	while(1);
  	}
	
  	// Set up NCO waveforms
  	NCO_T *s1 = init_nco(1000.0/32000, 0.0);
  	NCO_T *s2 = init_nco(1000.0/32000, 3*M_PI/2);
  	
	// Set the NCO amplitude to ensure DAC doesn't overflow
	s1->amp = 0.9;
  	s2->amp = 0.9;

  	// Coefficients for FIR lowpass decimation filter
  	float32_t pCoeffs[] = {
		0.000894733, 0.0010742, 0.00159809, 0.0021691, 0.00272184,
	       0.0031718, 0.00342232, 0.00336847, 0.00291526, 0.00198869,
	       0.000552561, -0.00137972, -0.00373259, -0.00636332, -0.00906144,
	       -0.0115575, -0.0135376, -0.0146663, -0.0146133, -0.0130853,
	       -0.00985679, -0.00480003, 0.00209268, 0.0106942, 0.0207392,
	       0.0318318, 0.0434665, 0.0550586, 0.0659845, 0.0756268,
	       0.0834209, 0.0888985, 0.0917246, 0.0917246, 0.0888985,
	       0.0834209, 0.0756268, 0.0659845, 0.0550586, 0.0434665,
	       0.0318318, 0.0207392, 0.0106942, 0.00209268, -0.00480003,
	       -0.00985679, -0.0130853, -0.0146133, -0.0146663, -0.0135376,
	       -0.0115575, -0.00906144, -0.00636332, -0.00373259, -0.00137972,
	       0.000552561, 0.00198869, 0.00291526, 0.00336847, 0.00342232,
	       0.0031718, 0.00272184, 0.0021691, 0.00159809, 0.0010742,
	       0.000894733
	};

  	// Initialization for CMSIS DSP ARM FIR Decimation filter	
  	arm_fir_decimate_init_f32(&s, numTaps, M, pCoeffs, pState, blockSize);
  	arm_fir_decimate_init_f32(&s3, numTaps, M, pCoeffs, pState2, blockSize);

        // Set size for ARM FFT
        uint32_t fftSize = 1024;
    
        // Allocate FFT arrays
        float32_t *fft_array = (float32_t *)malloc(2*fftSize*sizeof(float32_t));
        float32_t *fft_output = (float32_t *)malloc(fftSize*sizeof(float32_t));
  
        // Check for allocation errors
        if (fft_array == NULL || fft_output == NULL) {
	    flagerror(MEMORY_ALLOCATION_ERROR);
	    while(1);
        } 

        // List of window coefficients
        float32_t win[] = {
    
		0.438676, 0.440048, 0.44142, 0.442792, 0.444164,
		0.445536, 0.446908, 0.448279, 0.449651, 0.451023,
		0.452394, 0.453766, 0.455137, 0.456509, 0.45788,
		0.459251, 0.460622, 0.461993, 0.463363, 0.464734,
		0.466105, 0.467475, 0.468845, 0.470215, 0.471585,
		0.472954, 0.474324, 0.475693, 0.477062, 0.47843,
		0.479799, 0.481167, 0.482535, 0.483903, 0.485271,
		0.486638, 0.488005, 0.489372, 0.490738, 0.492104,
		0.49347, 0.494835, 0.496201, 0.497565, 0.49893,
		0.500294, 0.501658, 0.503021, 0.504384, 0.505747,
		0.507109, 0.508471, 0.509833, 0.511194, 0.512554,
		0.513915, 0.515274, 0.516634, 0.517993, 0.519351,
		0.520709, 0.522067, 0.523424, 0.52478, 0.526136,
		0.527492, 0.528847, 0.530201, 0.531555, 0.532909,
		0.534262, 0.535614, 0.536966, 0.538317, 0.539667,
		0.541018, 0.542367, 0.543716, 0.545064, 0.546412,
		0.547759, 0.549105, 0.550451, 0.551796, 0.55314,
		0.554484, 0.555827, 0.55717, 0.558511, 0.559852,
		0.561193, 0.562532, 0.563871, 0.565209, 0.566547,
		0.567883, 0.569219, 0.570554, 0.571889, 0.573222,
		0.574555, 0.575887, 0.577219, 0.578549, 0.579879,
		0.581208, 0.582535, 0.583863, 0.585189, 0.586514,
		0.587839, 0.589163, 0.590486, 0.591807, 0.593129,
		0.594449, 0.595768, 0.597086, 0.598404, 0.59972,
		0.601036, 0.60235, 0.603664, 0.604977, 0.606288,
		0.607599, 0.608909, 0.610217, 0.611525, 0.612832,
		0.614137, 0.615442, 0.616746, 0.618048, 0.61935,
		0.62065, 0.62195, 0.623248, 0.624545, 0.625842,
		0.627137, 0.628431, 0.629724, 0.631015, 0.632306,
		0.633595, 0.634884, 0.636171, 0.637457, 0.638742,
		0.640026, 0.641308, 0.642589, 0.643869, 0.645148,
		0.646426, 0.647703, 0.648978, 0.650252, 0.651525,
		0.652796, 0.654066, 0.655335, 0.656603, 0.65787,
		0.659135, 0.660399, 0.661661, 0.662922, 0.664182,
		0.665441, 0.666698, 0.667954, 0.669209, 0.670462,
		0.671714, 0.672964, 0.674213, 0.675461, 0.676707,
		0.677952, 0.679196, 0.680438, 0.681679, 0.682918,
		0.684156, 0.685392, 0.686627, 0.68786, 0.689092,
		0.690323, 0.691552, 0.692779, 0.694005, 0.69523,
		0.696453, 0.697674, 0.698894, 0.700112, 0.701329,
		0.702544, 0.703758, 0.70497, 0.706181, 0.70739,
		0.708597, 0.709803, 0.711007, 0.71221, 0.713411,
		0.71461, 0.715808, 0.717004, 0.718198, 0.719391,
		0.720582, 0.721772, 0.72296, 0.724146, 0.72533,
		0.726513, 0.727694, 0.728873, 0.730051, 0.731226,
		0.732401, 0.733573, 0.734743, 0.735912, 0.737079,
		0.738245, 0.739408, 0.74057, 0.74173, 0.742888,
		0.744044, 0.745199, 0.746352, 0.747503, 0.748652,
		0.749799, 0.750944, 0.752088, 0.753229, 0.754369,
		0.755507, 0.756643, 0.757777, 0.758909, 0.76004,
		0.761168, 0.762295, 0.763419, 0.764542, 0.765663,
		0.766781, 0.767898, 0.769013, 0.770126, 0.771237,
		0.772346, 0.773453, 0.774558, 0.775661, 0.776762,
		0.77786, 0.778957, 0.780052, 0.781145, 0.782236,
		0.783325, 0.784411, 0.785496, 0.786578, 0.787659,
		0.788737, 0.789814, 0.790888, 0.79196, 0.79303,
		0.794098, 0.795164, 0.796227, 0.797289, 0.798348,
		0.799406, 0.800461, 0.801514, 0.802564, 0.803613,
		0.804659, 0.805703, 0.806745, 0.807785, 0.808823,
		0.809858, 0.810891, 0.811922, 0.812951, 0.813978,
		0.815002, 0.816024, 0.817044, 0.818061, 0.819076,
		0.820089, 0.8211, 0.822108, 0.823114, 0.824118,
		0.825119, 0.826119, 0.827115, 0.82811, 0.829102,
		0.830092, 0.831079, 0.832065, 0.833047, 0.834028,
		0.835006, 0.835981, 0.836955, 0.837926, 0.838894,
		0.83986, 0.840824, 0.841785, 0.842744, 0.8437,
		0.844655, 0.845606, 0.846555, 0.847502, 0.848446,
		0.849388, 0.850327, 0.851264, 0.852199, 0.85313,
		0.85406, 0.854987, 0.855911, 0.856833, 0.857752,
		0.858669, 0.859584, 0.860496, 0.861405, 0.862312,
		0.863216, 0.864117, 0.865017, 0.865913, 0.866807,
		0.867699, 0.868587, 0.869474, 0.870357, 0.871238,
		0.872117, 0.872993, 0.873866, 0.874736, 0.875604,
		0.87647, 0.877333, 0.878193, 0.87905, 0.879905,
		0.880757, 0.881607, 0.882453, 0.883298, 0.884139,
		0.884978, 0.885814, 0.886647, 0.887478, 0.888306,
		0.889131, 0.889954, 0.890774, 0.891591, 0.892405,
		0.893217, 0.894026, 0.894832, 0.895636, 0.896437,
		0.897234, 0.89803, 0.898822, 0.899612, 0.900399,
		0.901183, 0.901964, 0.902743, 0.903518, 0.904291,
		0.905061, 0.905828, 0.906593, 0.907355, 0.908113,
		0.908869, 0.909623, 0.910373, 0.91112, 0.911865,
		0.912607, 0.913346, 0.914082, 0.914815, 0.915545,
		0.916272, 0.916997, 0.917719, 0.918437, 0.919153,
		0.919866, 0.920576, 0.921283, 0.921988, 0.922689,
		0.923387, 0.924083, 0.924775, 0.925465, 0.926151,
		0.926835, 0.927516, 0.928194, 0.928869, 0.92954,
		0.930209, 0.930875, 0.931538, 0.932198, 0.932855,
		0.933509, 0.93416, 0.934808, 0.935453, 0.936095,
		0.936734, 0.93737, 0.938003, 0.938633, 0.93926,
		0.939884, 0.940505, 0.941123, 0.941738, 0.942349,
		0.942958, 0.943564, 0.944166, 0.944766, 0.945362,
		0.945956, 0.946546, 0.947133, 0.947718, 0.948299,
		0.948877, 0.949452, 0.950023, 0.950592, 0.951158,
		0.95172, 0.95228, 0.952836, 0.953389, 0.953939,
		0.954486, 0.95503, 0.955571, 0.956109, 0.956643,
		0.957174, 0.957703, 0.958228, 0.95875, 0.959268,
		0.959784, 0.960296, 0.960806, 0.961312, 0.961815,
		0.962314, 0.962811, 0.963304, 0.963795, 0.964282,
		0.964766, 0.965246, 0.965724, 0.966198, 0.966669,
		0.967137, 0.967602, 0.968063, 0.968522, 0.968977,
		0.969428, 0.969877, 0.970322, 0.970765, 0.971204,
		0.971639, 0.972072, 0.972501, 0.972927, 0.97335,
		0.973769, 0.974186, 0.974599, 0.975009, 0.975415,
		0.975818, 0.976218, 0.976615, 0.977009, 0.977399,
		0.977786, 0.97817, 0.97855, 0.978927, 0.979301,
		0.979672, 0.980039, 0.980403, 0.980764, 0.981121,
		0.981475, 0.981826, 0.982174, 0.982518, 0.982859,
		0.983196, 0.983531, 0.983862, 0.984189, 0.984514,
		0.984835, 0.985153, 0.985467, 0.985778, 0.986086,
		0.986391, 0.986692, 0.98699, 0.987284, 0.987575,
		0.987863, 0.988148, 0.988429, 0.988707, 0.988981,
		0.989252, 0.98952, 0.989785, 0.990046, 0.990303,
		0.990558, 0.990809, 0.991057, 0.991301, 0.991542,
		0.99178, 0.992014, 0.992245, 0.992472, 0.992697,
		0.992917, 0.993135, 0.993349, 0.99356, 0.993767,
		0.993971, 0.994171, 0.994369, 0.994563, 0.994753,
		0.99494, 0.995124, 0.995304, 0.995481, 0.995655,
		0.995825, 0.995992, 0.996155, 0.996315, 0.996472,
		0.996625, 0.996775, 0.996921, 0.997064, 0.997204,
		0.99734, 0.997473, 0.997602, 0.997728, 0.997851,
		0.99797, 0.998086, 0.998199, 0.998308, 0.998414,
		0.998516, 0.998615, 0.99871, 0.998802, 0.998891,
		0.998976, 0.999058, 0.999136, 0.999211, 0.999283,
		0.999351, 0.999416, 0.999478, 0.999536, 0.99959,
		0.999641, 0.999689, 0.999733, 0.999774, 0.999812,
		0.999846, 0.999877, 0.999904, 0.999928, 0.999948,
		0.999965, 0.999979, 0.999989, 0.999996, 1,
		1, 0.999996, 0.999989, 0.999979, 0.999965,
		0.999948, 0.999928, 0.999904, 0.999877, 0.999846,
		0.999812, 0.999774, 0.999733, 0.999689, 0.999641,
		0.99959, 0.999536, 0.999478, 0.999416, 0.999351,
		0.999283, 0.999211, 0.999136, 0.999058, 0.998976,
		0.998891, 0.998802, 0.99871, 0.998615, 0.998516,
		0.998414, 0.998308, 0.998199, 0.998086, 0.99797,
		0.997851, 0.997728, 0.997602, 0.997473, 0.99734,
		0.997204, 0.997064, 0.996921, 0.996775, 0.996625,
		0.996472, 0.996315, 0.996155, 0.995992, 0.995825,
		0.995655, 0.995481, 0.995304, 0.995124, 0.99494,
		0.994753, 0.994563, 0.994369, 0.994171, 0.993971,
		0.993767, 0.99356, 0.993349, 0.993135, 0.992917,
		0.992697, 0.992472, 0.992245, 0.992014, 0.99178,
		0.991542, 0.991301, 0.991057, 0.990809, 0.990558,
		0.990303, 0.990046, 0.989785, 0.98952, 0.989252,
		0.988981, 0.988707, 0.988429, 0.988148, 0.987863,
		0.987575, 0.987284, 0.98699, 0.986692, 0.986391,
		0.986086, 0.985778, 0.985467, 0.985153, 0.984835,
		0.984514, 0.984189, 0.983862, 0.983531, 0.983196,
		0.982859, 0.982518, 0.982174, 0.981826, 0.981475,
		0.981121, 0.980764, 0.980403, 0.980039, 0.979672,
		0.979301, 0.978927, 0.97855, 0.97817, 0.977786,
		0.977399, 0.977009, 0.976615, 0.976218, 0.975818,
		0.975415, 0.975009, 0.974599, 0.974186, 0.973769,
		0.97335, 0.972927, 0.972501, 0.972072, 0.971639,
		0.971204, 0.970765, 0.970322, 0.969877, 0.969428,
		0.968977, 0.968522, 0.968063, 0.967602, 0.967137,
		0.966669, 0.966198, 0.965724, 0.965246, 0.964766,
		0.964282, 0.963795, 0.963304, 0.962811, 0.962314,
		0.961815, 0.961312, 0.960806, 0.960296, 0.959784,
		0.959268, 0.95875, 0.958228, 0.957703, 0.957174,
		0.956643, 0.956109, 0.955571, 0.95503, 0.954486,
		0.953939, 0.953389, 0.952836, 0.95228, 0.95172,
		0.951158, 0.950592, 0.950023, 0.949452, 0.948877,
		0.948299, 0.947718, 0.947133, 0.946546, 0.945956,
		0.945362, 0.944766, 0.944166, 0.943564, 0.942958,
		0.942349, 0.941738, 0.941123, 0.940505, 0.939884,
		0.93926, 0.938633, 0.938003, 0.93737, 0.936734,
		0.936095, 0.935453, 0.934808, 0.93416, 0.933509,
		0.932855, 0.932198, 0.931538, 0.930875, 0.930209,
		0.92954, 0.928869, 0.928194, 0.927516, 0.926835,
		0.926151, 0.925465, 0.924775, 0.924083, 0.923387,
		0.922689, 0.921988, 0.921283, 0.920576, 0.919866,
		0.919153, 0.918437, 0.917719, 0.916997, 0.916272,
		0.915545, 0.914815, 0.914082, 0.913346, 0.912607,
		0.911865, 0.91112, 0.910373, 0.909623, 0.908869,
		0.908113, 0.907355, 0.906593, 0.905828, 0.905061,
		0.904291, 0.903518, 0.902743, 0.901964, 0.901183,
		0.900399, 0.899612, 0.898822, 0.89803, 0.897234,
		0.896437, 0.895636, 0.894832, 0.894026, 0.893217,
		0.892405, 0.891591, 0.890774, 0.889954, 0.889131,
		0.888306, 0.887478, 0.886647, 0.885814, 0.884978,
		0.884139, 0.883298, 0.882453, 0.881607, 0.880757,
		0.879905, 0.87905, 0.878193, 0.877333, 0.87647,
		0.875604, 0.874736, 0.873866, 0.872993, 0.872117,
		0.871238, 0.870357, 0.869474, 0.868587, 0.867699,
		0.866807, 0.865913, 0.865017, 0.864117, 0.863216,
		0.862312, 0.861405, 0.860496, 0.859584, 0.858669,
		0.857752, 0.856833, 0.855911, 0.854987, 0.85406,
		0.85313, 0.852199, 0.851264, 0.850327, 0.849388,
		0.848446, 0.847502, 0.846555, 0.845606, 0.844655,
		0.8437, 0.842744, 0.841785, 0.840824, 0.83986,
		0.838894, 0.837926, 0.836955, 0.835981, 0.835006,
		0.834028, 0.833047, 0.832065, 0.831079, 0.830092,
		0.829102, 0.82811, 0.827115, 0.826119, 0.825119,
		0.824118, 0.823114, 0.822108, 0.8211, 0.820089,
		0.819076, 0.818061, 0.817044, 0.816024, 0.815002,
		0.813978, 0.812951, 0.811922, 0.810891, 0.809858,
		0.808823, 0.807785, 0.806745, 0.805703, 0.804659,
		0.803613, 0.802564, 0.801514, 0.800461, 0.799406,
		0.798348, 0.797289, 0.796227, 0.795164, 0.794098,
		0.79303, 0.79196, 0.790888, 0.789814, 0.788737,
		0.787659, 0.786578, 0.785496, 0.784411, 0.783325,
		0.782236, 0.781145, 0.780052, 0.778957, 0.77786,
		0.776762, 0.775661, 0.774558, 0.773453, 0.772346,
		0.771237, 0.770126, 0.769013, 0.767898, 0.766781,
		0.765663, 0.764542, 0.763419, 0.762295, 0.761168,
		0.76004, 0.758909, 0.757777, 0.756643, 0.755507,
		0.754369, 0.753229, 0.752088, 0.750944, 0.749799,
		0.748652, 0.747503, 0.746352, 0.745199, 0.744044,
		0.742888, 0.74173, 0.74057, 0.739408, 0.738245,
		0.737079, 0.735912, 0.734743, 0.733573, 0.732401,
		0.731226, 0.730051, 0.728873, 0.727694, 0.726513,
		0.72533, 0.724146, 0.72296, 0.721772, 0.720582,
		0.719391, 0.718198, 0.717004, 0.715808, 0.71461,
		0.713411, 0.71221, 0.711007, 0.709803, 0.708597,
		0.70739, 0.706181, 0.70497, 0.703758, 0.702544,
		0.701329, 0.700112, 0.698894, 0.697674, 0.696453,
		0.69523, 0.694005, 0.692779, 0.691552, 0.690323,
		0.689092, 0.68786, 0.686627, 0.685392, 0.684156,
		0.682918, 0.681679, 0.680438, 0.679196, 0.677952,
		0.676707, 0.675461, 0.674213, 0.672964, 0.671714,
		0.670462, 0.669209, 0.667954, 0.666698, 0.665441,
		0.664182, 0.662922, 0.661661, 0.660399, 0.659135,
		0.65787, 0.656603, 0.655335, 0.654066, 0.652796,
		0.651525, 0.650252, 0.648978, 0.647703, 0.646426,
		0.645148, 0.643869, 0.642589, 0.641308, 0.640026,
		0.638742, 0.637457, 0.636171, 0.634884, 0.633595,
		0.632306, 0.631015, 0.629724, 0.628431, 0.627137,
		0.625842, 0.624545, 0.623248, 0.62195, 0.62065,
		0.61935, 0.618048, 0.616746, 0.615442, 0.614137,
		0.612832, 0.611525, 0.610217, 0.608909, 0.607599,
		0.606288, 0.604977, 0.603664, 0.60235, 0.601036,
		0.59972, 0.598404, 0.597086, 0.595768, 0.594449,
		0.593129, 0.591807, 0.590486, 0.589163, 0.587839,
		0.586514, 0.585189, 0.583863, 0.582535, 0.581208,
		0.579879, 0.578549, 0.577219, 0.575887, 0.574555,
		0.573222, 0.571889, 0.570554, 0.569219, 0.567883,
		0.566547, 0.565209, 0.563871, 0.562532, 0.561193,
		0.559852, 0.558511, 0.55717, 0.555827, 0.554484,
		0.55314, 0.551796, 0.550451, 0.549105, 0.547759,
		0.546412, 0.545064, 0.543716, 0.542367, 0.541018,
		0.539667, 0.538317, 0.536966, 0.535614, 0.534262,
		0.532909, 0.531555, 0.530201, 0.528847, 0.527492,
		0.526136, 0.52478, 0.523424, 0.522067, 0.520709,
		0.519351, 0.517993, 0.516634, 0.515274, 0.513915,
		0.512554, 0.511194, 0.509833, 0.508471, 0.507109,
		0.505747, 0.504384, 0.503021, 0.501658, 0.500294,
		0.49893, 0.497565, 0.496201, 0.494835, 0.49347,
		0.492104, 0.490738, 0.489372, 0.488005, 0.486638,
		0.485271, 0.483903, 0.482535, 0.481167, 0.479799,
		0.47843, 0.477062, 0.475693, 0.474324, 0.472954,
		0.471585, 0.470215, 0.468845, 0.467475, 0.466105,
		0.464734, 0.463363, 0.461993, 0.460622, 0.459251,
		0.45788, 0.456509, 0.455137, 0.453766, 0.452394,
		0.451023, 0.449651, 0.448279, 0.446908, 0.445536,
		0.444164, 0.442792, 0.44142, 0.440048, 0.438676
	    };

	// Windowing scale factor
    	float W0 = 1017.9;
    /*
   	* Normally we avoid printf()... especially once we start actually
   	* processing streaming samples.  This is here to illustrate the
   	* use of printf for debugging programs.
   	*
   	* To see the printf output, connect to the ST-Link serial port.
   	* Use: 115200 8N1
   	*/
  	printf("Starting execution using %d samples per input block.\n",nsamp);
  	
	/*
   	* Infinite Loop to process the data stream, "nsamp" samples at a time
   	*/
  	while(1){
    		/*
     		* Ask for a block of ADC samples to be put into the working buffer
     		*   getblock() will wait here until the input buffer is filled...  On return
     		*   we work on the new data buffer... then return here to wait for
     		*   the next block
     		*/
    		getblock(input);
    		
		counter++; // Keep track of current block number
    		
		/*
     		* signal processing code to calculate the required output buffers
     		*/

    		DIGITAL_IO_SET();  // Use a scope on PD0 to measure execution time

    		// Generate NCO with proper center frequency from button
		nco_set_frequency(s1, f0/32000.0);
		nco_set_frequency(s2, f0/32000.0);

		// Code to calculate NCO waveform samples (+-exp(2*pi*f0))
		nco_get_samples(s1, output1, nsamp);
		nco_get_samples(s2, output2, nsamp);

		// Multiply input by 2 NCO waveforms (MIXER)
		for (i = 0; i < nsamp; i++) {
			output1[i] = input[i] * output1[i];
			output2[i] = -input[i] * output2[i];
		}

		// Send input through FIR Decimation filter
		arm_fir_decimate_f32(&s, output1, decBlock1, blockSize);
		arm_fir_decimate_f32(&s3, output2, decBlock2, blockSize);

		// Put alternating samples from filter output real,imag into fft array
		for (i = 0; i < nsamp/M ; i++) { // decBlock len = nsamp/M
			fft_array[(counter-1)*2*nsamp/M+(i*2)] = decBlock1[i]*win[(counter-1)*nsamp/M+i];
			fft_array[(counter-1)*2*nsamp/M+(i*2)+1] = decBlock2[i]*win[(counter-1)*nsamp/M+i];
		}

		// Once 1024 decimated samples are collected then do FFT
		if (counter > 7) {
			// CALCULATE FFT IN HERE
		    	arm_cfft_f32( &arm_cfft_sR_f32_len1024, fft_array, 0, 1); // Calculate FFT
		    	arm_cmplx_mag_f32(fft_array, fft_output, fftSize); // Get magnitude of vectors
		    	arm_scale_f32(fft_output, 4/(W0), fft_output, fftSize); // Scale for output to DAC
			
			counter = 0; // Reset block counter
		}

		// Put FFT results to output
		for (i = 0; i < nsamp ; i++) {
			
			if ((i > (fftSize-192)) || (i < 192)) {
				// Mask out frequencies out of range
				output2[i] = -1.0;

			} else if (i < 512) {
				// Show -1kHz -> 0
				output2[i] = fft_output[1023-(511-i)]-1.0;

			} else if (i >= 512) {
				// Show 0 -> +1kHz
				output2[i] = fft_output[i-512]-1.0;

			}
			
			// Generate triangle wave for reference
			if (i < 192) {
				// Mask out frequencies out of range
				output1[i] = -1.0;

			}else if (i > (fftSize-192)) {
				// Mask out frequencies out of range
				output1[i] = -1.0;

			} else {
				// Line equation using magnitude of frequency step
				output1[i] = (i*(2.0/fftSize)-1.0);  
			}

		}


    		DIGITAL_IO_RESET();	// (falling edge....  done processing data )

    		/*
     		* pass the processed working buffer back for DAC output
     		*/
    		putblockstereo(output1, output2);
    		
		// Cycle through center frequency of mixer, f0, by pushing the button
    		if (KeyPressed) 
		{
			KeyPressed = RESET;
			button_count++;

			// If f0 greater than 15kHz then reset to DC
			if (button_count > 15) {
				button_count = 0;
			}

			// Calculate frequency using button count
			f0=button_count*1000;

			// String to display to LCD
  			char dispString[8];

			// Format the display string with center frequency for output
  			sprintf(dispString,"%d",f0);

			// Clear the display
  			BSP_LCD_GLASS_DisplayString((uint8_t *)"        ");

			// Display the center frequency
			BSP_LCD_GLASS_DisplayString((uint8_t *)dispString);


		}
	}

}
