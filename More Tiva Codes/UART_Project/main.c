#include "TM4C123.h"                    // Device header
#include "UART.h"
#include <stdlib.h>
#include <string.h>

/*
	Date Created: 15 - 07 - 2019
	Date Updated: 16 - 07 - 2019
	A simple program to enable serial communication between PC and
	TM4C123 using UART.
	
	Operation:
	- If user types red in the serial window, red LED comes on
	- If user types green in the serial window, green LED comes on
	- If user types blue in the serial window, blue LED comes on
	- If user types blink in the serial window, white LED blinks
	- If user doesn't type any of the above, all LEDs go off
	
	Additional software: PuTTY (for serial communication)
*/

void PLL_Init(void);
void PortF_Init(void);

int main(){
	
	int red,green,blue,blink;
	char readStr[MAX];
	char ledAction[MAX];
	
	UART_Init();
	PLL_Init();
	PortF_Init();
	
	__disable_irq();
	//Systick
	SysTick->LOAD = 4000000 - 1;
	__enable_irq();
	
	while(1){
		
		UART_OutString("--> Enter a string: ");
		UART_InString(readStr);
		strcpy(ledAction,readStr);
		
		red = strcmp(ledAction,"red");
	  green = strcmp(ledAction,"green");
		blue = strcmp(ledAction,"blue");
		blink = strcmp(ledAction,"blink");
		
		if (!blink){
			GPIOF->DATA = 0x00;
			UART_OutString("White LED is blinking\n\r");
			SysTick->CTRL = 7; //enable systick, enable systick interrupt and clock
		}
		else{
			
			if (!red){
				UART_OutString("Red LED is on\n\r");
				GPIOF->DATA = 0x02;
			}
			else if (!green){
				UART_OutString("Green LED is on\n\r");
				GPIOF->DATA = 0x08;
			}
			else if (!blue){
				UART_OutString("Blue LED is on\n\r");
				GPIOF->DATA = 0x04;
			}
			
			else {
				GPIOF->DATA = 0x00;
				UART_OutString("Invalid Option, all LEDs off\n\r");
			}
			SysTick->CTRL = 4; //disable systick, no systick interrupt, use clock
		}
	}
}
//SysTick ISR
void SysTick_Handler(){
	GPIOF->DATA ^= 0x0E; //blink white LED
}

//PLL_Init
//PLL Initialization for 20MHz clock speed
void PLL_Init(){
	//20MHz PLL frequency
	SYSCTL->RCC &= ~0x7000000; 
	SYSCTL->RCC |= 0x4000000; 
	UART0->CTL |= 0x01; //Enable UART for use
}

//PortF_Init
//PortF Initialization
void PortF_Init(){
	SYSCTL->RCGCGPIO |= 0x20; //Enable clock for port F
	GPIOF->AFSEL &= 0x00; //No alternate function
	GPIOF->DIR |= 0x0E; //PF3-PF1 as outputs
	GPIOF->DEN |= 0x0E; //Enable digital functions for PF3-PF1
}

