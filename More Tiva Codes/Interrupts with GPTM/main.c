/*
Generating periodic interrupts with GPTM timers
*/

#include "TM4C123.h"                    // Device header
//*******Setups**********
void Timer_Init(void);
//***********************

volatile int Timer_flag = 0;
volatile int GPIO_flag = 0;

int main(){
	
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
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
	
	while (1){
		
		if (Timer_flag){
			GPIOF->DATA ^= 0x08;
			Timer_flag = 0;
			
		}
		if (GPIO_flag){
			GPIOF->DATA &= 0x00;
			GPIO_flag = 0;
		}
	}
}

void TIMER0A_Handler(){
	Timer_flag = 1;
	TIMER0->ICR |= 0x01; //Clear timeout flag
}

void Timer_Init(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x2; //Periodic Timer Mode
  TIMER0->TAMR |= 0x00; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000; // 16kHz --> 1ms
	TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	if ((GPIOF->MIS & 0x10) == 0x10) {
		TIMER0->IMR &= ~0x01; //Disable interrupts for the timer
		GPIO_flag = 1;
	}
	else if ((GPIOF->MIS & 0x01) == 0x01) {
		TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	}
	
	GPIOF->ICR |= 0x11; 
}
