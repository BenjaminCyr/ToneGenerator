/* File: cosine.h
 * Author: Benjamin Cyr
 * Date: April 9, 2017
 */


#ifndef _COSINE_H_
#define _COSINE_H_

// Q18.14 Fixed Point Numbers for Approximation
#define PI 51472 //3.1416015625 
#define PI_OVER_2 25736 //1.5708007813
#define TWO 32768 // 2
#define ONE 16384 // 1
#define C1 ONE // 1
#define C2 -8192 // -1/2!
#define C3 683 // 1/4!
#define C4 -23  // -1/6!

#include <stdint.h>

/* Structure for the wave generator */
typedef struct wave_gen_t { 
 	/* Frequency is in Q28.4, because more decimal bits would cause overflow.*/
	uint32_t frequency;
 	/* Sampling rate is in Hz */
	uint32_t sampling_rate;
	/* Wave goes from 0 to amplitude */
	uint32_t amplitude;

 	/* Used by algorithm */
	uint32_t current_sample;
	uint32_t rem;
	uint32_t correction;
} wave_gen_t;

// Prototypes
int cos_taylor(int q1814_int);
void reset_wave_gen (wave_gen_t * gen); 
uint16_t getNextSample(wave_gen_t * gen);

#endif
