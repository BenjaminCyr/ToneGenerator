/* File: LED.h
 * Author: Benjamin Cyr
 * Date: April 15, 2017
 */

#define LED_OFF 0x00  
#define LED_G 0x01  
#define LED_O 0x02  
#define LED_R 0x04  
#define LED_B 0x08  

#define LED_OG LED_O | LED_G
#define LED_RG LED_R | LED_G
#define LED_BG LED_B | LED_G
#define LED_RO LED_R | LED_O
#define LED_BO LED_B | LED_O
#define LED_BR LED_B | LED_R

#define LED_ROG LED_RO | LED_G
#define LED_BOG LED_BO | LED_G
#define LED_BRG LED_BR | LED_G
#define LED_BRO LED_BR | LED_O

#define LED_BROG LED_BRO | LED_G

typedef enum LED_t {
	ALL,
	GREEN,
	ORANGE,
	RED,
	BLUE
} LED_t;

void LED_Initialize(void);
void LED_WritePattern(uint8_t pattern);
void LED_Toggle(LED_t LED_selection);

