/*
	Interrupts program 3b (Better code)
	Description::
	If one switch is pressed, white LED blinks
	If another is pressed, LED goes off

*/
#include "TM4C123.h"                    // Device header

int main(){
	SYSCTL->RCGCGPIO |= 0x20;
	
	//unlock PF0
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	
	__disable_irq();
	
	//Systick
	SysTick->LOAD = 16000000 - 1;
	
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	__enable_irq();
	
	while (1){}
		
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	if ((GPIOF->MIS & 0x10) == 0x10){
		SysTick->CTRL = 7; //enable, systick interrupt and clock
	}
	else if ((GPIOF->MIS & 0x01) == 0x01){
		SysTick->CTRL = 5; //enable, no systick interrupt, use clock
		GPIOF->DATA &= 0x00; //LED off
	}
	GPIOF->ICR |= 0x11; 
}

void SysTick_Handler(){
	GPIOF->DATA ^= 0x0E; //blink white LED
}
