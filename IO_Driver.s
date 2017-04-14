; Benjamin Cyr
; IO_Driver.s
; February 21, 2017

;Clocks
RCC		EQU	0x40023800		;Clock Enable Address
AHB1ENR	EQU	0x30			;GPIO Clock Enable register
	
;GPIO
GPIOA	EQU	0x40020000		;GPIOA Address
GPIOD	EQU	0x40020C00		;GPIOD Address
MODER 	EQU 0x00			;Mode register offset
ODR		EQU 0x14			;Output Data register offset
IDR		EQU	0x10			;Input Data register offset
BSRRL	EQU 0x18			;Set register offset
BSRRH	EQU	0x1A			;Reset register offset
	
;System Configuration
SYS_CFG	EQU	0x40013800		;System Configuration Address
EXTICR1 EQU 0x08			;EXTI Control Register 1 offset
	
;External Interrupt
EXTI 	EQU 0x40013C00		;EXTI Register Address
IMR		EQU 0x00			;Interrupt Mask Register offset
RTSR	EQU 0x08			;Rising-edge Trigger Set Register offset
PR		EQU 0x14			;Pending Request Register Offset

;NVIC
NVIC	EQU 0xE000E100		;Register for NVIC peripheral
ISER0	EQU 0x00			;Interrupt Set Enable Register
EXTI0B	EQU 0x40			;Bit 6 for EXTI0 interrupt
NVIC_PR	EQU	0xE000E400		;Register for NVIC priority
IPR1	EQU	0x04			;Interrupt Priority Register 1
BUTTPRI	EQU 0x00100000		;Set EXTI0 priority to 1

		AREA IO_Driver, CODE
		THUMB
			
		EXPORT LED_Initialize
		EXPORT LED_WritePattern
		EXPORT LED_Toggle
		EXPORT Button_Initialize
		EXPORT Button_SetInterruptEXTI0
		EXPORT Button_Read
		
;--------------------------------------------------------------------
;LED_Initialize: Sets up PD12-PD15 as outputs for LEDs
;--------------------------------------------------------------------
LED_Initialize
		ldr	r0, =RCC			
		ldr r1, [r0,#AHB1ENR]	;Load the Clock control bit
		orr r1, #0x08			;Enable Clock on GPIO D
		str r1, [r0,#AHB1ENR]
		
		ldr r0, =GPIOD
		ldr r1, [r0,#MODER]		;Set as Outputs:
		bic r1, #0xAA000000		;Clear bits 31,29,27,25
		orr r1, #0x55000000		;Set bits 30,28,26,24
		str r1, [r0,#MODER]		;Store Mode register
		bx lr

;--------------------------------------------------------------------
;LED_WritePattern: Writes a 4-bit pattern to B-R-O-G LEDs
;Input: r0 - The 4-bit pattern to write
;--------------------------------------------------------------------
LED_WritePattern
		ldr r1, =GPIOD			;load address to GPIOD
		lsl r0, #12				;shift r0 for ORR
		ldrh r2, [r1, #ODR]
		bic r2, #0xF000
		orr r2, r0 
		strh r2, [r1, #ODR]		
		bx lr
		
;--------------------------------------------------------------------
;LED_Toggle: Toggles the selected LED
;Input: r0 - The LED to change state (All-0, G-1, O-2, R-3, B-4)
;--------------------------------------------------------------------
LED_Toggle
		push {r4}
		ldr r3, =GPIOD
		teq r0, #0
		beq Alltog
		add r4, r0, #11			;Add 11 so shift is appropriate
		mov r2, #1
		lsl r2, r4				;Select Appropriate LED
		b tog
Alltog	mov r2, #0xF000			;Select All LEDs
tog		ldrh r4, [r3, #ODR]
		eor r4, r2
		strh r4, [r3, #ODR]		;Toggle the selected LED
		pop {r4}
		bx lr
		
;--------------------------------------------------------------------
;Button_Initialize: Initializes the Button PA0
;--------------------------------------------------------------------
Button_Initialize
		ldr	r0, =RCC			
		ldr r1, [r0,#AHB1ENR]	;Load the Clock control bit
		orr r1, #0x01			;Enable Clock on GPIO A
		str r1, [r0,#AHB1ENR]
		
		ldr r0, =GPIOA
		ldr r1, [r0, #MODER]	;Load register for button.
		bic r1, #0x03			;Set button as an input
		str r1, [r0, #MODER]
		bx lr
		
;--------------------------------------------------------------------
;Button_SetInterruptEXTI0: Sets Button PA0 as interrupt EXTI0
;--------------------------------------------------------------------
Button_SetInterruptEXTI0
		ldr r0, =SYS_CFG	
		ldr r1, [r0, #EXTICR1]		;Load EXTI Control Register
		bic r1, #0x0F
		str r1, [r0, #EXTICR1]		;Set EXTI0 to read PA0 (button)
		
		ldr r0, =EXTI
		ldr r1, [r0, #IMR]			
		orr r1, #0x01
		str r1, [r0, #IMR]			;Enable Interrupt Mask for EXTI0
		ldr r1, [r0, #RTSR]
		orr r1, #0x01
		str r1, [r0, #RTSR]			;Set Interrupt on Rising Edge
		
		ldr r0, =NVIC				
		mov r1, #EXTI0B
		str r1, [r0, #ISER0]		;Enable EXTI0 (bit 6) in NVIC
		
		ldr r0, =NVIC_PR
		mov r1, #BUTTPRI
		str r1, [r0, #IPR1]			;Set the priority of the button
		bx lr
		
;--------------------------------------------------------------------
;Button_Read: Reads debounced button signal from an interrupt context. 
;Output: r0 - whether the button is still pressed after debounce.
;--------------------------------------------------------------------
Button_Read
		ldr r0, =0x00008000		;Load Delay for Switch Debounce
debnce	subs r0, #1
		bne debnce
		ldr r1, =GPIOA
		ldr r0, [r1,#IDR]		;Check if line still High
		and r0, #0x01			;State of PA0
		bx lr
		
		END