;Assembly code to toggle LED with button press

;External hardware:
;-RC filter {R = 1Megaohms, C = 100nF}
;-Pullup resistor of 10k
;-Tactile Button
;-LED

;Working (Expected)
;First press, LED on
;Second press, LED off
;Third press, LED on
;etc.

;Working (Observed)
;Project works such that a button press
;triggers the LED then it goes off.
;This is because the processor is too fast
;in jumping from the LED routines back to
;the Loop. (In high level languages, conditionals and flags would've done the trick)
;[My thoughts are also on bouncing effects]

;PB0 --> Button
;PB1 --> LED

SYSCTL_RCGCGPIO_R	EQU	0x400FE608	
GPIO_PORTB_DIR_R	EQU	0x40005400
GPIO_PORTB_DEN_R	EQU	0x4000551C
GPIO_PORTB_DATA_R	EQU	0x400053FC

		AREA  |.text|, CODE, READONLY, ALIGN=2
		THUMB
		EXPORT Main
Main
		BL GPIOB_Init

Loop
		LDR R1, =GPIO_PORTB_DATA_R
		LDR R0,[R1]
		CMP R0,#0x00
		BEQ LED_ON
		CMP R0,#0x02
		BEQ LED_OFF
		B Loop

GPIOB_Init
		;Activate clock for Port B
		LDR	R1, =SYSCTL_RCGCGPIO_R
		LDR R0,[R1]
		ORR	R0,R0,#0x02
		STR	R0,[R1]
		;PB0 as input, PB1 as output
		LDR 	R1, =GPIO_PORTB_DIR_R
		MOV		R0,#0x02
		STR		R0,[R1]
		;Enable digital functions 
		LDR		R1, =GPIO_PORTB_DEN_R
		MOV		R0, #0x03
		STR		R0,[R1]
		
		BX		LR
		
LED_ON
		LDR R1, =GPIO_PORTB_DATA_R
		MOV R0,#0x02
		STR R0,[R1]
		BX LR
		
LED_OFF
		LDR R1, =GPIO_PORTB_DATA_R
		MOV R0,#0x00
		STR R0,[R1]
		BX LR
		
		ALIGN
		END
		