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
#define VOLUME 1

#include "stm32f4xx.h"  
#include "..\Includes\cs43l22.h"
#include "..\Includes\stm32f4_discovery.h"
#include "SystemClockConfig.c"
#include "cosine.h"
#include "LED.h"
#include "button.h"
#include "AU_FightSong.h"
#include "ZeldasLullaby.h"

typedef struct song_t {
	uint32_t size;
	uint32_t bpm;
	uint32_t beat_duration;
	notes_t notes;
}

void timerSetup(void);
void startTimer(void);
void stopTimer(void);
void DACSetup(void);
void codecClockSetup(void);

// Globals
uint16_t DAC_value;

uint32_t beat_ticks;	//Counter for Flashing LED
uint8_t current_song;	//Tells whether sound is stopped
uint8_t current_note; 	//Current index of the frequency

wave_gen_t generator = {0};
wave_gen_t * gen_ptr = &generator;

song_t song = {0};
song_t * song_ptr = &song;

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
	beat_ticks = song.notes[0].duration*song.beat_duration;
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
			switch(current_song) {
			case 0: 
				cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_OFF);
				startTimer();
				song.notes = ZELDAS_LULLABY;
				song.size = ZELDAS_LULLABY_SIZE
				setBPM(song_ptr, ZELDAS_LULLABY_BPM);
				current_note = 0;
				LED_WritePattern(notes[current_note].LED_pattern);
				current_song = 1;
				break;
			case 1:
				song.notes = AU_FIGHTSONG;
				song.size = AU_FIGHTSONG_SIZE
				setBPM(song_ptr, AU_FIGHTSONG_BPM);
				current_note = 0;
				LED_WritePattern(notes[current_note].LED_pattern);
				current_song = 1;
				break;
			default:
				cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_ON);
				stopTimer();
				LED_WritePattern(LED_OFF);
				current_song = 0;
			}
		}
		EXTI->PR |= 0x01; //Clear Pending Bit
}

void TIM2_IRQHandler() {
	//Clear Pending Bit
	TIM2->SR &= ~TIM_SR_UIF;
	if (beat_ticks-- <= 0) {
		current_note = current_note < song.size - 1? current_note + 1 : 0;
		beat_ticks = song.notes[current_note].duration*song.beat_duration;
		gen_ptr->frequency = notes[current_note].frequency;
		reset_wave_gen(gen_ptr);
		LED_WritePattern(notes[current_note].LED_pattern);
	}

	//Set next value to DAC
	DAC_value = getNextSample(gen_ptr);
	DAC->DHR12R1 = DAC_value;
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

void setBPM(song_t * song, uint32_t bpm) {
	song->bpm = bpm;
	song->beat_duration = SAMPLING_RATE*60/bpm;
}

//I don't know why HAL doesn't set this up, 
//but it breaks without it.
void SysTick_Handler() {
	HAL_IncTick();
}
