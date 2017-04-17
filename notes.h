/* File: notes.h
 * Author: Benjamin Cyr
 * Date: April 15, 2017
 */
 
#ifndef _NOTES_H_
#define _NOTES_H_

#include "LED.h"

//Note durations
#define SIXTEENTH_NOTE 1
#define EIGHTH_NOTE 2
#define DOTTED_EIGHTH_NOTE 3
#define QUARTER_NOTE 4
#define DOTTED_QUARTER_NOTE 6
#define HALF_NOTE 8
#define DOTTED_HALF_NOTE 12
#define WHOLE_NOTE 16

//Note Frequencies
#define REST_NOTE 0  
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
#define Ds5_NOTE 9956 
#define Eb5_NOTE Ds5_NOTE
#define E5_NOTE 10548 
#define F5_NOTE 11175 
#define Fs5_NOTE 11840 
#define Gb5_NOTE Fs5_NOTE 
#define G5_NOTE 12544 
#define Gs5_NOTE 13290
#define Ab5_NOTE Gs5_NOTE
#define A5_NOTE 14080
#define As5_NOTE 14917
#define Bb5_NOTE As5_NOTE
#define B5_NOTE 15804

#define C6_NOTE 16744
#define Cs6_NOTE 17740
#define Db6_NOTE Cs6_NOTE
#define D6_NOTE 18795
#define Ds6_NOTE 19912
#define Eb6_NOTE Ds6_NOTE
#define E6_NOTE 21096
#define F6_NOTE 22351
#define Fs6_NOTE 23680
#define Gb6_NOTE Fs6_NOTE
#define G6_NOTE 25088
#define Gs6_NOTE 26580
#define Ab6_NOTE Gs6_NOTE
#define A6_NOTE 28160
#define As6_NOTE 29835
#define Bb6_NOTE As6_NOTE
#define B6_NOTE 31608
#define C7_NOTE 33488


// Predefined Note_t
#define REST(NOTE)		{REST_NOTE, NOTE, LED_OFF}	
#define C4(NOTE)		{C4_NOTE, NOTE, LED_R}
#define Cs4(NOTE)		{Cs4_NOTE, NOTE, LED_O}
#define Db4(NOTE)		{Db4_NOTE, NOTE, LED_O}
#define D4(NOTE)		{D4_NOTE, NOTE, LED_RO}
#define Ds4(NOTE)		{Ds4_NOTE, NOTE, LED_G}
#define Eb4(NOTE)		{Eb4_NOTE, NOTE, LED_G}
#define E4(NOTE)		{E4_NOTE, NOTE, LED_RG}
#define F4(NOTE) 		{F4_NOTE, NOTE, LED_OG}
#define Fs4(NOTE) 		{Fs4_NOTE, NOTE, LED_ROG}
#define Gb4(NOTE) 		{Gb4_NOTE, NOTE, LED_ROG}
#define G4(NOTE) 		{G4_NOTE, NOTE, LED_B}
#define Gs4(NOTE)		{Gs4_NOTE, NOTE, LED_BR}
#define Ab4(NOTE)		{Ab4_NOTE, NOTE, LED_BR}
#define A4(NOTE)		{A4_NOTE, NOTE, LED_BO}
#define As4(NOTE)		{As4_NOTE, NOTE, LED_BRO}
#define Bb4(NOTE)		{Bb4_NOTE, NOTE, LED_BRO}
#define B4(NOTE)		{B4_NOTE, NOTE, LED_BG}

#define C5(NOTE)		{C5_NOTE, NOTE, LED_R}
#define Cs5(NOTE)		{Cs5_NOTE, NOTE, LED_O}
#define Db5(NOTE)		{Db5_NOTE, NOTE, LED_O}
#define D5(NOTE)		{D5_NOTE, NOTE, LED_RO}
#define Ds5(NOTE)		{Ds5_NOTE, NOTE, LED_G}
#define Eb5(NOTE)		{Eb5_NOTE, NOTE, LED_G}
#define E5(NOTE)		{E5_NOTE, NOTE, LED_RG}
#define F5(NOTE) 		{F5_NOTE, NOTE, LED_OG}
#define Fs5(NOTE) 		{Fs5_NOTE, NOTE, LED_ROG}
#define Gb5(NOTE) 		{Gb5_NOTE, NOTE, LED_ROG}
#define G5(NOTE) 		{G5_NOTE, NOTE, LED_B}
#define Gs5(NOTE)		{Gs5_NOTE, NOTE, LED_BR}
#define Ab5(NOTE)		{Ab5_NOTE, NOTE, LED_BR}
#define A5(NOTE)		{A5_NOTE, NOTE, LED_BO}
#define As5(NOTE)		{As5_NOTE, NOTE, LED_BRO}
#define Bb5(NOTE)		{Bb5_NOTE, NOTE, LED_BRO}
#define B5(NOTE)		{B5_NOTE, NOTE, LED_BG}

#define C6(NOTE)		{C6_NOTE, NOTE, LED_R}
#define Cs6(NOTE)		{Cs6_NOTE, NOTE, LED_O}
#define Db6(NOTE)		{Db6_NOTE, NOTE, LED_O}
#define D6(NOTE)		{D6_NOTE, NOTE, LED_RO}
#define Ds6(NOTE)		{Ds6_NOTE, NOTE, LED_G}
#define Eb6(NOTE)		{Eb6_NOTE, NOTE, LED_G}
#define E6(NOTE)		{E6_NOTE, NOTE, LED_RG}
#define F6(NOTE) 		{F6_NOTE, NOTE, LED_OG}
#define Fs6(NOTE) 		{Fs6_NOTE, NOTE, LED_ROG}
#define Gb6(NOTE) 		{Gb6_NOTE, NOTE, LED_ROG}
#define G6(NOTE) 		{G6_NOTE, NOTE, LED_B}
#define Gs6(NOTE)		{Gs6_NOTE, NOTE, LED_BR}
#define Ab6(NOTE)		{Ab6_NOTE, NOTE, LED_BR}
#define A6(NOTE)		{A6_NOTE, NOTE, LED_BO}
#define As6(NOTE)		{As6_NOTE, NOTE, LED_BRO}
#define Bb6(NOTE)		{Bb6_NOTE, NOTE, LED_BRO}
#define B6(NOTE)		{B6_NOTE, NOTE, LED_BG}

#define C7(NOTE)		{C7_NOTE, NOTE, LED_R}

typedef struct note_t {
	uint32_t frequency;
	uint32_t duration;
	uint32_t LED_pattern;
} note_t;

#endif

