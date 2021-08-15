; Assembly code to turn red LED on
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
		BL		LIGHT_ON
		B		loop

GPIOF_Init
		;Activate clock of port F in RCGCGPIO register
		LDR		R1, =SYSCTL_RCGCGPIO_R
		LDR     R0,[R1]
		ORR		R0,R0,#0x20
		STR		R0,[R1]
		;================================================
		
		;Set GPIO pin as output
		LDR 	R1, =GPIO_PORTF_DIR_R
		MOV		R0,#0x02
		STR		R0,[R1]
		;=================================================
		
		;Enable digital functions 
		LDR		R1, =GPIO_PORTF_DEN_R
		MOV		R0, #0x02
		STR		R0,[R1]
		;==================================================
		BX		LR
		
LIGHT_ON
		
		;Write to GPIODATA register
		LDR		R1, =GPIO_PORTF_DATA_R
		MOV		R0,#0x02
		STR		R0,[R1]
		BX		LR
		;===================================================
		ALIGN
		END