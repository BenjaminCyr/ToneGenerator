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

#define DAC_MAX 4095
#define SAMPLING_RATE 20000
#define VOLUME 1

#include "stm32f4xx.h"  
#include "..\Includes\cs43l22.h"
#include "..\Includes\stm32f4_discovery.h"
#include "SystemClockConfig.c"
#include "cosine.h"

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

uint32_t one_second_ticks; //Keep track of One Second button press
uint32_t LED_ticks;			//Counter for Flashing LED
uint8_t stopped;			//Tells whether sound is stopped
uint8_t current_frequency; 	//Current index of the frequency
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
		
uint8_t LED_patterns[] = {0x00, 0x04, 0x02, 0x06, 0x01, 0x05, 0x03, 0x07}; 
uint16_t LED_speeds[] = {20000, 15000, 11000, 9000, 6000, 4000, 2500, 1500}; 
		 
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
	current_frequency = 0;
	LED_ticks = 0;
	gen_ptr->frequency = frequencies[0];
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
				stopped = 0;
				one_second_ticks = 0;
			}
			else if(one_second_ticks < SAMPLING_RATE) {
				//Stop if less than one second since last press
				stopTimer();
				cs43l22_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_ON);
				stopped = 1;
			}
			else {
				// Else change frequency
				current_frequency = current_frequency < 7 ? current_frequency + 1 : 0;
				gen_ptr->frequency = frequencies[current_frequency];
				reset_wave_gen(gen_ptr);
				one_second_ticks = 0;
			}
		}
		LED_WritePattern(LED_patterns[current_frequency]);
		EXTI->PR |= 0x01; //Clear Pending Bit
}

void TIM2_IRQHandler() {
	//Clear Pending Bit
	TIM2->SR &= ~TIM_SR_UIF;
	//Count towards one second
	if (one_second_ticks < SAMPLING_RATE) one_second_ticks++;
	//Count down LED_ticks
	if (LED_ticks-- == 0) {
		//If 0, toggle LED and reset
		LED_ticks = LED_speeds[current_frequency];
		LED_Toggle(BLUE);
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

//I don't know why HAL doesn't set this up, 
//but it breaks without it.
void SysTick_Handler() {
	HAL_IncTick();
}
