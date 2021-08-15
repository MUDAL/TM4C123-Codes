/*

*/

#include "TM4C123.h"                    // Device header
//*******Setups**********
void Timer_Init(void);
//***********************

unsigned long start;
unsigned long stop;

int main(){
	
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC |= 0x400000; //Enable system clock divisor
	SYSCTL->RCC &= ~0x1000000; //System clock = 16/2 = 8MHz
	
	//unlock PF0
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	
	
	
	__disable_irq();
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 1<<5; //set priority 1
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	Timer_Init();
	__enable_irq();
	
	while(1){
	}
	
}

void Timer_Init(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x00; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000; // 16kHz --> 1ms
	start = TIMER0->TAILR;
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	if ((GPIOF->MIS & 0x10) == 0x10) {
		TIMER0->CTL &= ~0x01; //Disable timer
		stop = TIMER0->TAV;
		TIMER0->TAV = TIMER0->TAILR;
	}
	else if ((GPIOF->MIS & 0x01) == 0x01) {
		TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	}
	
	GPIOF->ICR |= 0x11; 
}
