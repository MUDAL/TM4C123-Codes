;Assembly code for blinking onboard white LED

;Defining register addresses
SYSCTL_RCGCGPIO_R	EQU	0x400FE608	
GPIO_PORTF_DIR_R	EQU	0x40025400
GPIO_PORTF_DEN_R	EQU	0x4002551C
GPIO_PORTF_DATA_R	EQU	0x400253FC
	
	AREA |.text|,CODE,READONLY,ALIGN=2
	THUMB
	EXPORT main

main
	BL		GPIOF_Init

loop
	BL		TOGGLE_LED
	LDR		R0,=0xF4240
	BL		DELAY ;short delay
	B		loop

GPIOF_Init
		;Activate clock of port F in RCGCGPIO register
		LDR		R1, =SYSCTL_RCGCGPIO_R
		LDR     R0,[R1]
		ORR		R0,R0,#0x20
		STR		R0,[R1]
		;================================================
		
		;Set GPIO pins 3,2,1 as output
		LDR 	R1, =GPIO_PORTF_DIR_R
		MOV		R0,#0x0E
		STR		R0,[R1]
		;=================================================
		
		;Enable digital functions 
		LDR		R1, =GPIO_PORTF_DEN_R
		MOV		R0, #0x0E
		STR		R0,[R1]
		;==================================================
		BX		LR

TOGGLE_LED
		;toggle white LED subroutine
		LDR		R1, =GPIO_PORTF_DATA_R
		LDR		R0,[R1]
		EOR		R0,R0,#0x0E
		STR		R0,[R1]
		BX		LR

DELAY
		;delay function
		SUBS	R0,R0,#1
		BNE		DELAY
		BX		LR
		ALIGN
		END