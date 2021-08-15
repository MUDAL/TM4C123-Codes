/*
	Interrupts program 4b (Better code)
	Description::
	If one switch is pressed, red LED blinks
	If it is pressed again, LED goes off

*/
#include "TM4C123.h"                    // Device header

int main(){
	SYSCTL->RCGCGPIO |= 0x28;
	
	GPIOD->DIR &= ~0x01; //make PD0 an input pin
	GPIOF->DIR |= 0x0E;
	GPIOD->DEN |= 0x01;
	GPIOF->DEN |= 0x0E;
	
	__disable_irq();
	
	//Systick
	SysTick->LOAD = 16000000 - 1;
	
	GPIOD->IS &= ~0x01; //make PD0 edge sensitive
	GPIOD->IBE &= ~0x01; //not enabling interrupts for both edges
	GPIOD->IEV &= ~0x01; //falling edge detection
	GPIOD->ICR |= 0x01; //clear any prior interrupt
	GPIOD->IM |= 0x01; //unmask interrupt
	
	NVIC->IP[3] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x08; //enable IRQ3
	
	__enable_irq();
	
	while (1){}
}

void GPIOD_Handler(void){
	//GPIOD ISR code:
	static int flag;
	if ((GPIOD->MIS & 0x01) == 0x01){
		
		if (!flag){
			SysTick->CTRL = 7; //enable, systick interrupt and clock
			flag = 1;
		}
		else if (flag){
			SysTick->CTRL = 5; //enable, no systick interrupt, use clock
			GPIOF->DATA &= 0x00; //LED off
			flag = 0;
		}
		
	}
	GPIOD->ICR |= 0x01; 
}

void SysTick_Handler(){
	GPIOF->DATA ^= 0x02; //blink red LED
}
