/* File: cosine.c
 * Author: Benjamin Cyr
 * Date: April 9, 2017
 */

#include "cosine.h"

uint16_t getNextSample(wave_gen_t * gen) {
	uint32_t value, temp, x;

	// Just to avoid divide by zero errors
	if (gen->sampling_rate == 0) return 0;

	/* When current_sample == sampling_rate*16: gaurenteed to be at
	 * beginning of cycle. Able to reset and start over.
	 * Makes it so there is less chance of an overflow somewhere.*/
	if (gen->current_sample/gen->sampling_rate > 15) {
		reset_wave_gen(gen);
	}

 	// Frequency is in Q28.4, because more decimal bits would cause overflow.
	temp = gen->frequency*2*PI >> 4;	

	/* Keeping track of remainder so that correct frequency is produced. Without
	 * the correction, the loss of precision in the division will produce a 
	 * lower frequency.*/
	gen->rem += temp % gen->sampling_rate;
	if (gen->rem > gen->sampling_rate) {
		gen->correction++;  
		gen->rem -= gen->sampling_rate;
	}

	// Calculate the correct value to be "cosined" in Q18.14
	x = temp/gen->sampling_rate*gen->current_sample + gen->correction;

	// Cosine the value and scale to the DAC values from 0 to DAC_MAX
	value = ((cos_taylor(x) + ONE)*gen->amplitude)/TWO;

	// Prepare for the next sample.
	gen->current_sample++;
	return value;
}

//Taylor series approximation of cosine
int cos_taylor(int q1814_int) {
	int x, x2, quadrant, cosine;
	x = q1814_int; //Q18.14 format
	quadrant = ( x / PI_OVER_2 ) % 4; //Figure out which quadrant
	x = x % PI; //Bring within range of good approximations
	if (quadrant == 1 || quadrant == 3){
		x = x - PI;  //If Quadrant 1 or 3, x starts at -PI/2 and goes to 0
	}
	x2 = x*x >> 14; //x^2

	//Cosine approximation: cos(x) = C1+C2*x^2+C3*x^4+C4*x^6
	cosine = C1 + (x2*(C2 + (x2*(C3 + (x2*C4 >> 14)) >> 14)) >> 14); 
	if (quadrant == 1 || quadrant == 2){
		cosine = -cosine; //If Quadrant 1 or 2, cosine is negative
	}
	return cosine;
}

void reset_wave_gen (wave_gen_t * gen) {
	gen->current_sample = 0;
	gen->rem = 0;
	gen->correction = 0;
}
