/* File: button.h
 * Author: Benjamin Cyr
 * Date: April 15, 2017
 */
 
 #ifndef _BUTTON_H_
 #define _BUTTON_H_

void Button_Initialize(void);
void Button_SetInterruptEXTI0(void);
uint8_t Button_Read(void);

#endif
