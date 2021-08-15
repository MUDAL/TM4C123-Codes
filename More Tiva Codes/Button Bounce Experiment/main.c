#include "TM4C123.h"                    // Device header
#include <stdlib.h>
#include <string.h>

char buttonPressed[6] = "00000";
void UART_OutChar(unsigned char data);

int main(){
	
	int colour_variable = 0;
	unsigned char character;
	
	//======= CLOCK CONFIG ==================================
	SYSCTL->RCGCGPIO |= 0x32; //Clock for ports F,B and E
	SYSCTL->RCGCUART |= 0x01; //Clock for UART0
	SYSCTL->RCGCGPIO |= 0x01; //Clock for Port A
	//20MHz PLL frequency
	SYSCTL->RCC &= ~0x7000000; 
	SYSCTL->RCC |= 0x4000000;
	//=========================================================
	
	//==== unlock PB ==========
	GPIOB->LOCK = 0x4C4F434B;
	GPIOB->CR = 0x0C;
	//==========================
	
	GPIOB->DIR &= 0x00; //Configure all port B pins as inputs
	GPIOB->DEN |= 0xFF; //Enables digital functions for PB7 to PB0
	//GPIOB->PUR |= 0xFF; //Enables internal pullup for PB7 to PB0
	GPIOB->AFSEL &= 0x00; //Disable alternate functions
	GPIOF->DIR |= 0x0E; //Configure PF3-1 as outputs
	GPIOF->DEN |= 0x1F;
	
	//====== PORTB INTERRUPTS ======================================
	GPIOB->IS &= ~0xFF; //make PB0-7 edge sensitive
	GPIOB->IBE &= ~0xFF; //disabling both edge interrupts for PB0-7
	GPIOB->IEV &= ~0xFF; //falling edge detection for PB0-7
	GPIOB->ICR |= 0xFF; //clear any prior interrupt
	GPIOB->IM |= 0xFF; //unmask interrupt
	
	NVIC->IP[1] = 1<<5; //Set priority 1 for port B interrupts
	NVIC->ISER[0] = 0x02; //enable IRQ1
	__enable_irq();
	//=================================================================
	
	//===== UART0 CONFIG ============================================
	GPIOA->DEN |= 0x03; //Enable digital functions for PA0 and PA1
	GPIOA->AMSEL &= ~0x03; //Disable analog functions for PA0 and PA1
	GPIOA->AFSEL |= 0x03; //Enable alternate functions for PA0 and PA1
	GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011; //Select UART function
	
	//BRD(baud-rate divisor = 10.8507) and baud rate of 115200
	UART0->CTL &= ~0x01; //Disable UART before use
	UART0->IBRD |= 0x0A; //Integer part of 10 (from datasheet)
	UART0->FBRD |= 0x36; //Fractional part of 54 (from datasheet)
	UART0->LCRH |= 0x00000070; //8 bit word length and Enable FIFOs
	UART0->CTL |= 0x01; //Enable UART for use
	//==============================================================
	
	while(1){
		switch(colour_variable){
			case 1:
				character = 'X';
				break;
		}
		if (!strcmp(buttonPressed,"10000")){
			colour_variable = 1;
			UART_OutChar(character);
			strcpy(buttonPressed,"00000");
		}
	}
}

void GPIOB_Handler(void){
	//GPIOB ISR code:
	if ((GPIOB->MIS & 0x01) == 0x01) buttonPressed[0] = '1';
	GPIOB->ICR |= 0xFF; 
}

void UART_OutChar(unsigned char data){
	while((UART0->FR & 0x20) == 0x20);
	UART0->DR = data;
}
