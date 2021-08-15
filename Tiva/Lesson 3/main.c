#include "TM4C123.h"                    // Device header
#define CLOCK 16000000

volatile int count = 0;

int main(){
	SYSCTL->RCGCGPIO |= 0x20;
	GPIOF-> DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	
	//Disable PLL and using 16MHz clock source
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
	//Configure systick
	__disable_irq();
	SysTick->LOAD = CLOCK - 1;
	SysTick->CTRL = (1<<2)|(1<<1)|(1<<0); //enable, use interrupt, use system clock
	
	__enable_irq();
	
	while (1){
		if (count == 200){
			GPIOF->DATA |= 0x04;
		}
		//GPIOF->DATA = 0x00;
	}	
}

void SysTick_Handler(){
	count++;
	GPIOF->DATA ^= 0x02;
}
