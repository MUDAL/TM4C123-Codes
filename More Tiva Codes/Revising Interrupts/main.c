/*
- Date:: 01 - 08 - 2019
- Revision of interrupts
- The global variable was made volatile to curb the effects of
compiler optimizations
- The code doesn't have a debouncing function
*/
#include "TM4C123.h"                    // Device header
volatile int flag = 0;

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
	
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	__enable_irq();
	
	while (1){
		if (!flag) {GPIOF->DATA = 0x02;}
		else {GPIOF->DATA = 0x0E;}
	}
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	if ((GPIOF->MIS & 0x10) == 0x10 && !flag) {flag = 1;}
	else if ((GPIOF->MIS & 0x10) == 0x10 && flag) {flag = 0;}
	
	GPIOF->ICR |= 0x11; 
}

