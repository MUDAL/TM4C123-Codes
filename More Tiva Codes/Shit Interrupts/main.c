/*
An experiment with periodic interrupts
RJX would know what to uncomment
The pseudo delays are used to observe how the interrupts work
Pseudo delays are not advisable for practical systems
*/

#include "TM4C123.h"                    // Device header

volatile int GPIO_flag = 0;
void Delay(unsigned long delayMs);

int main(){
	
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	SYSCTL->RCGCGPIO |= 0x10; //Enable clock for port E
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
	//======= unlock PF0 =======
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	//==========================
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	GPIOE->AFSEL &= 0x00; //No alternate function
	GPIOE->DIR |= 0x0E; //PE3-PE1 as outputs
	GPIOE->DEN |= 0x0E; //Enable digital functions for PE3-PE1
	
	__disable_irq();
	SysTick->LOAD = 16000000 - 1;
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 1<<5; //set priority 1
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	SysTick->CTRL = 7; //enable systick interrupt
	__enable_irq();
	
	while (1){
		
		GPIOF->DATA |= 0x04;
		Delay(60);
		GPIOF->DATA &= ~0x04;
		Delay(60);
		
		/*if (GPIO_flag){
			SysTick->CTRL = 7; //enable systick interrupt
			GPIO_flag = 0;
		}*/
	}
		
}

void SysTick_Handler(){
	GPIOE->DATA ^= 0x04;
	//Delay(80);
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	if ((GPIOF->MIS & 0x10) == 0x10) {
		//SysTick->CTRL = 7; //enable systick interrupt
		//GPIO_flag = 1;
		GPIOE->DATA |= 0x04;
		Delay(1000);
		GPIOE->DATA &= ~0x04;
		//Delay(1000);
		
	}
	/*else if ((GPIOF->MIS & 0x01) == 0x01) {
		//SysTick->CTRL = 5; //disable systick interrupt
		GPIOE->DATA &= 0x00; //Clear port E
		GPIOF->DATA &= 0x00; //Clear port F
	}*/
	
	GPIOF->ICR |= 0x11; 
}

void Delay(unsigned long delayMs){
	unsigned long i;
	for (i = 0; i < delayMs * 11416; i++){}
}

