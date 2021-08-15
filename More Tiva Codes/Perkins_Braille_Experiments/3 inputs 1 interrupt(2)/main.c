/*
DATE:: 15 - 10 - 2019
DESCRIPTION:: Triple simultaneous inputs - 1 output

\about 40ms tolerance between the inputs
\key uVision project for Perkins Brailler code

*/
#include "TM4C123.h"                    // Device header

volatile int trigger1 = 0;
volatile int trigger2 = 0;
volatile int trigger3 = 0;

void timerInit(void);

int main(){
	
	SYSCTL->RCGCGPIO |= 0x28; //clock for ports D and F
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC |= 0x400000; //Enable system clock divisor
	SYSCTL->RCC &= ~0x1000000; //System clock = 16/2 = 8MHz
	
	GPIOD->DIR &= ~0x07; //make PD0-2 input pins
	GPIOF->DIR |= 0x0E; //PF1-3 as outputs
	GPIOD->DEN |= 0x07; //digital functions for PD0-2
	GPIOF->DEN |= 0x0E; //digital functions for PF1-3
	
	__disable_irq();
	timerInit();
	
	GPIOD->IS &= ~0x07; //make PD0-2 edge sensitive
	GPIOD->IBE &= ~0x07; //disabling interrupts for both edges of PD0-2
	GPIOD->IEV &= ~0x07; //falling edge detection for PD0-2
	GPIOD->ICR |= 0x07; //clear any prior interrupt
	GPIOD->IM |= 0x07; //unmask interrupt
	
	NVIC->IP[3] = 1<<5; //set priority 1
	NVIC->ISER[0] = 0x08; //enable IRQ3
	__enable_irq();
	
	while (1){
		//The main code is executed before a timer interrupt gets raised
		//i.e both buttons are pressed with a few millisecs between them
		if (trigger1 & trigger2 & trigger3){
			GPIOF->DATA ^= 0x0E;
			trigger1 = 0;
			trigger2 = 0;
			trigger3 = 0;
		}
	}
}

void TIMER0A_Handler(){
	/*This ISR is executed if both buttons are not 
	pressed seemingly at the same time*/
	
	if(trigger1) 			trigger1 = 0;
	else if(trigger2) trigger2 = 0;
	else if(trigger3) trigger3 = 0;
	TIMER0->ICR |= 0x01;
}

void GPIOD_Handler(void){
	//GPIOD ISR code:
	TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	if ((GPIOD->MIS & 0x01) == 0x01) 			trigger1 = 1;
	else if ((GPIOD->MIS & 0x02) == 0x02) trigger2 = 1;
	else if ((GPIOD->MIS & 0x04) == 0x04) trigger3 = 1;
	
	GPIOD->ICR |= 0x07; 
}

void timerInit(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 320000 - 1; // 8kHz --> 1ms
	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
	
}
