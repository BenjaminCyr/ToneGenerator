/* File: main.c
 * Author: Benjamin Cyr
 * Date: April 9, 2017
 */

// Timer values: Tout = (ARR+1)x(PSC+1)x(1/Fclk)
#define TIM2_ARR_VALUE 419
#define TIM2_PSC_VALUE 9
// 420*10/84000000 = .05 ms = 20000 Hz

#define TIM3_ARR_VALUE 6
#define TIM3_PSC_VALUE 0
// 7*1/84000000 =  83ns = 12 MHz

#define DAC_MAX 2048
#define SAMPLING_RATE 20000
#define VOLUME 1
#define SONG_SIZE 73

#define BPM 300
#define BEAT SAMPLING_RATE*60/BPM
#define SIXTEENTH_BEAT BEAT/4

//Note durations
#define SIXTEENTH_NOTE SIXTEENTH_BEAT
#define EIGHTH_NOTE 2*SIXTEENTH_BEAT
#define DOTTED_EIGHTH_NOTE 3*SIXTEENTH_BEAT
#define QUARTER_NOTE BEAT
#define DOTTED_QUARTER_NOTE 6*SIXTEENTH_BEAT
#define HALF_NOTE 2*BEAT
#define DOTTED_HALF_NOTE 3*BEAT
#define WHOLE_NOTE 4*BEAT

//Note Frequencies
#define REST 0  
#define C4_NOTE 4186  // C4 261.625 Hz
#define Cs4_NOTE 4435 // C#/Db4 277.1875 Hz
#define Db4_NOTE Cs4_NOTE 
#define D4_NOTE	4699  // D4 293.6875 Hz
#define Ds4_NOTE 4978 // D#/Eb4 311.125 Hz
#define Eb4_NOTE Ds4_NOTE 
#define E4_NOTE	5274  // E4 329.625 Hz
#define F4_NOTE	5588  // F4 349.25 Hz
#define Fs4_NOTE 5920  // F#/Gb4 370.00 Hz
#define Gb4_NOTE Fs4_NOTE  
#define G4_NOTE	6272  // G4 392.00 Hz
#define Gs4_NOTE 6645  // G#/Ab4 415.3125 Hz
#define Ab4_NOTE Gs4_NOTE  
#define A4_NOTE 7040  // A4 440.00 Hz
#define As4_NOTE 7459  // A#/Bb4 466.1875 Hz
#define Bb4_NOTE As4_NOTE 
#define B4_NOTE 7902  // B4 493.875 Hz
#define C5_NOTE 8372  // C5 523.25 Hz
#define Cs5_NOTE 8870 // C#/Db5 554.375 Hz
#define Db5_NOTE Cs5_NOTE 
#define D5_NOTE 9397 // D5 587.3125 Hz

#define ____ 0  
#define ___G 1  
#define __O_ 2  
#define __OG 3  
#define _R__ 4  
#define _R_G 5  
#define _RO_ 6  
#define _ROG 7  
#define B___ 8  
#define B__G 9  
#define B_O_ 10 
#define B_OG 11 
#define BR__ 12 
#define BR_G 13 
#define BRO_ 14 
#define BROG 15 

#include "stm32f4xx.h"  
#include "..\Includes\cs43l22.h"
#include "..\Includes\stm32f4_discovery.h"
#include "SystemClockConfig.c"
#include "cosine.h"

typedef struct Note_t {
	uint32_t frequency;
	uint32_t duration;
	uint32_t LED_pattern;
} Note_t;

typedef enum LED_t {
	ALL,
	GREEN,
	ORANGE,
	RED,
	BLUE
} LED_t;

void timerSetup(void);
void startTimer(void);
void stopTimer(void);
void DACSetup(void);
void codecClockSetup(void);
void LED_Initialize(void);
void LED_WritePattern(uint8_t pattern);
void LED_Toggle(LED_t LED_selection);
void Button_Initialize(void);
void Button_SetInterruptEXTI0(void);
uint8_t Button_Read(void);

// Globals
uint16_t DAC_value;

uint32_t beat_ticks;			//Counter for Flashing LED
uint8_t stopped;			//Tells whether sound is stopped
uint8_t current_note; 	//Current index of the frequency
// Frequencies in Q28.4
Note_t notes[] = 
		{{C4_NOTE, WHOLE_NOTE, _R__},
		{A4_NOTE, HALF_NOTE, B_O_},
		{G4_NOTE, HALF_NOTE, B___},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{F4_NOTE, HALF_NOTE, __OG},
		{D4_NOTE, QUARTER_NOTE, _RO_},
		{C4_NOTE, DOTTED_HALF_NOTE, _R__},	
		{REST, QUARTER_NOTE, ____},	
		{E4_NOTE, QUARTER_NOTE, _R_G},
		{E4_NOTE, HALF_NOTE, _R_G},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{G4_NOTE, HALF_NOTE, B___},
		{C4_NOTE, HALF_NOTE, _R__},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{F4_NOTE, HALF_NOTE, __OG},
		{G4_NOTE, QUARTER_NOTE, B___},
		{A4_NOTE, DOTTED_HALF_NOTE, B_O_},
		{REST, QUARTER_NOTE, ____},
		{C4_NOTE, WHOLE_NOTE, _R__},
		{A4_NOTE, HALF_NOTE, B_O_},
		{G4_NOTE, HALF_NOTE, B___},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{F4_NOTE, HALF_NOTE, __OG},
		{D4_NOTE, QUARTER_NOTE, _RO_},
		{C4_NOTE, DOTTED_HALF_NOTE, _R__},	
		{REST, QUARTER_NOTE, ____},
		{E4_NOTE, QUARTER_NOTE, _R_G},
		{Eb4_NOTE, HALF_NOTE, ___G},
		{E4_NOTE, QUARTER_NOTE, _R_G},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{E4_NOTE, QUARTER_NOTE, _R_G},
		{D4_NOTE, HALF_NOTE, _RO_},
		{G4_NOTE, QUARTER_NOTE, B___},
		{REST, QUARTER_NOTE, ____},
		{C5_NOTE, HALF_NOTE, _R__},
		{C5_NOTE, HALF_NOTE, _R__},
		{C5_NOTE, HALF_NOTE, _R__},
		{C5_NOTE, DOTTED_HALF_NOTE, _R__},
		{Bb4_NOTE, QUARTER_NOTE, BRO_},
		{A4_NOTE, HALF_NOTE, B_O_},
		{G4_NOTE, HALF_NOTE, B___},
		{F4_NOTE, QUARTER_NOTE, __OG},
		{F4_NOTE, HALF_NOTE, __OG},
		{G4_NOTE, QUARTER_NOTE, B___},
		{A4_NOTE, DOTTED_HALF_NOTE, B_O_},
		{REST, QUARTER_NOTE, ____},
		{D5_NOTE, QUARTER_NOTE, BROG},
		{D5_NOTE, QUARTER_NOTE, BROG},
		{C5_NOTE, HALF_NOTE, BR_G},
		{D5_NOTE, QUARTER_NOTE, BROG},
		{D5_NOTE, QUARTER_NOTE, BROG},
		{C5_NOTE, HALF_NOTE, BR_G},
		{Bb4_NOTE, QUARTER_NOTE, BRO_},
		{Bb4_NOTE, HALF_NOTE, BRO_},
		{A4_NOTE, QUARTER_NOTE, B_O_},
		{G4_NOTE, QUARTER_NOTE, B___},
		{REST, QUARTER_NOTE, ____},
		{C5_NOTE, QUARTER_NOTE, BR_G},
		{REST, QUARTER_NOTE, ____},
		{C4_NOTE, WHOLE_NOTE, _R__},
		{A4_NOTE, HALF_NOTE, B_O_},
		{G4_NOTE, HALF_NOTE, B___},
		{F4_NOTE, HALF_NOTE, __OG},
		{G4_NOTE, HALF_NOTE, B___},
		{A4_NOTE, HALF_NOTE, B_O_},
		{D5_NOTE, HALF_NOTE, BROG},
		{C5_NOTE, QUARTER_NOTE, _R__},
		{B4_NOTE, HALF_NOTE, B__G},
		{C5_NOTE, QUARTER_NOTE, _R__},
		{A4_NOTE, HALF_NOTE, B_O_},
		{G4_NOTE, HALF_NOTE, B___},
		{F4_NOTE, WHOLE_NOTE, __OG},
		{REST, WHOLE_NOTE, ____}
		
		
		
		
//		{REST, QUARTER_NOTE, ____},	
//		{C4_NOTE, WHOLE_NOTE, _R__},
//		{Cs4_NOTE, QUARTER_NOTE, __O_},
//		{D4_NOTE, QUARTER_NOTE, _RO_},
//		{Ds4_NOTE, QUARTER_NOTE, ___G},
//		{E4_NOTE, QUARTER_NOTE, _R_G},
//		{F4_NOTE, QUARTER_NOTE, __OG},
//		{Fs4_NOTE, QUARTER_NOTE, _ROG},
//		{G4_NOTE, QUARTER_NOTE, B___},
//		{Gs4_NOTE, QUARTER_NOTE, BR__},
//		{A4_NOTE, QUARTER_NOTE, B_O_},
//		{As4_NOTE, QUARTER_NOTE, BRO_},
//		{B4_NOTE, QUARTER_NOTE, B__G},
//		{C5_NOTE, QUARTER_NOTE, BR_G},
//		{Cs5_NOTE, QUARTER_NOTE, B_OG},
//		{D5_NOTE, QUARTER_NOTE, BROG},
	};		
		
wave_gen_t generator = {0};
wave_gen_t * gen_ptr = &generator;

int main(void){
	SystemClock_Config(); //Increase Clock Speed
	timerSetup();  				//Set up 20KHz timer
	codecClockSetup(); 		//Set up Clock pin to Codec Chip
	DACSetup();						//Set up DAC
	LED_Initialize();			//Initialize LEDs
	Button_Initialize(); 	//Initialize Button
	Button_SetInterruptEXTI0();		//Set Button as Interrupt
	
	//Initialize Audio Codec Chip
	cs43l22_Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, VOLUME, SAMPLING_RATE);
	
	//Mute to stop Buzzing
	cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_ON);
	
	//Initialize variables
	stopped = 1;
	current_note = 0;
	beat_ticks = notes[0].duration;
	gen_ptr->frequency = notes[0].frequency;
	gen_ptr->sampling_rate = SAMPLING_RATE;
	gen_ptr->amplitude = DAC_MAX;
	
	//Enable Interrupts
	__enable_irq();
	
		for (;;);
}

void EXTI0_IRQHandler() {
		//Debounce Button
		if (Button_Read()) {
			if(stopped) { 
				//Unmute and Start
				cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_OFF);
				startTimer();
				LED_WritePattern(notes[current_note].LED_pattern);
				stopped = 0;
			}
			else {
				//Stop if less than one second since last press
				cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_ON);
				stopTimer();
				LED_WritePattern(____);
				stopped = 1;
			}
		}
		EXTI->PR |= 0x01; //Clear Pending Bit
}

void TIM2_IRQHandler() {
	//Clear Pending Bit
	TIM2->SR &= ~TIM_SR_UIF;
	if (beat_ticks-- <= 0) {
		current_note = current_note < SONG_SIZE - 1? current_note + 1 : 0;
		beat_ticks = notes[current_note].duration;
		gen_ptr->frequency = notes[current_note].frequency;
		reset_wave_gen(gen_ptr);
		LED_WritePattern(notes[current_note].LED_pattern);
	}

	//Set next value to DAC
	DAC_value = getNextSample(gen_ptr);
	DAC->DHR12R1 = DAC_value & 0x0FFF;
}

void codecClockSetup() {
	//Initialize GPIO PC7 for special function
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	GPIOC->MODER |= GPIO_MODER_MODER7_1;
	GPIOC->AFR[0] &= ~0xF0000000;
	GPIOC->AFR[0] |= 0x20000000; 
	
	//Setup TIM3 Channel 2 to output to PC7
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->ARR = TIM3_ARR_VALUE;
	TIM3->PSC = TIM3_PSC_VALUE;
	TIM3->CCMR1 &= ~0xFF00;
	TIM3->CCMR1 |= 0x3000;
	TIM3->CCR2 = TIM3_ARR_VALUE;
	TIM3->CCER |= 0x30;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void timerSetup() { 
	//Initialize TIM2 and set as interrupt
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = TIM2_ARR_VALUE;
	TIM2->PSC = TIM2_PSC_VALUE;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
}

void startTimer() {
	//Start Timer
	TIM2->CR1 |= TIM_CR1_CEN;
}

void stopTimer() {
	//Stop Timer
	TIM2->CR1 &= ~TIM_CR1_CEN;
}

void DACSetup() {
	//Set up DAC at PA4
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	GPIOA->MODER |= GPIO_MODER_MODER4;
	DAC->CR |= DAC_CR_EN1;
	DAC->DHR12R1 = 0;
}

//I don't know why HAL doesn't set this up, 
//but it breaks without it.
void SysTick_Handler() {
	HAL_IncTick();
}
