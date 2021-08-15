/*
	Introduction to interrupts
	Systick interrupts
*/

#include "TM4C123.h"                    // Device header

int main(){
	SYSCTL->RCGCGPIO |= 0x20;
	GPIOF-> DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	
	//Disable PLL and using 16MHz clock source
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
	//Configure systick
	__disable_irq();
	SysTick->LOAD = 16000000 - 1;
	SysTick->CTRL = 7; //enable, use interrupt, use system clock
	
	__enable_irq();
	
	while (1){}	
}

void SysTick_Handler(){
	GPIOF->DATA ^= 0x0E;
}
