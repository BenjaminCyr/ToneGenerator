/* File: main.c
 * Author: Benjamin Cyr
 * Date: April 9, 2017
 */

#define DAC_MAX 4095

#include "stm32f4xx.h"  
#include "cosine.h"

// Globals
uint16_t DAC_value;
uint32_t working;

// Frequencies in Q28.4
uint32_t frequencies[] = 
		{4186,  // C4 261.625 Hz
		 4699,  // D4 293.6875 Hz
		 5274,  // E4 329.625 Hz
		 5588,  // F4 349.25 Hz
		 6272,  // G4 392.00 Hz
		 7040,  // A4 440.00 Hz
		 7902,  // B4 493.875 Hz
		 8372}; // C5 523.25 Hz
uint32_t sampling_rate = 20000;
		 
wave_gen_t generator = {0};
wave_gen_t * gen_ptr = &generator;

int main(void){
	gen_ptr->frequency = frequencies[7];
	gen_ptr->sampling_rate = sampling_rate;
	gen_ptr->amplitude = DAC_MAX;
	
	//SystemClock_Config();
	SysTick_Config(SystemCoreClock/sampling_rate);
	
	for (;;);
}

void SysTick_Handler() {
		DAC_value = getNextSample(gen_ptr);
}
