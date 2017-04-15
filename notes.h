/* File: notes.h
 * Author: Benjamin Cyr
 * Date: April 15, 2017
 */

#include "LED.h"

#define SAMPLING_RATE 20000

#define BPM 120
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

#define REST(NOTE)		{REST, NOTE, LED_OFF}	
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
#define C5(NOTE)		{C5_NOTE, NOTE, LED_BRG}
#define Cs5(NOTE)		{Cs5_NOTE, NOTE, LED_BOG}
#define Db5(NOTE)		{Db5_NOTE, NOTE, LED_BOG}
#define D5(NOTE)		{D5_NOTE, NOTE, LED_BROG}

typedef struct Note_t {
	uint32_t frequency;
	uint32_t duration;
	uint32_t LED_pattern;
} Note_t;


