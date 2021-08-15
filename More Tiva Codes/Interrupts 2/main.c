/*
	GPIO Interrupts
	Description::
	* If SW1 is pressed, an interrupt is triggered and a red LED comes on
	for a couple of seconds.
	* If SW2 is pressed, an interrupt is triggered and a white LED toggles
	4 times
	* If no switch is pressed, green LED stays on.
*/

#include "TM4C123.h"                    // Device header
void Delay(unsigned long delayMs);

int main(){
	SYSCTL->RCGCGPIO |= 0x20;
	//Unlock  PF0
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
		//Main code:
		//Green LED is on by default
		GPIOF->DATA |= 0x08;
	}	
}

void GPIOF_Handler(void){
	//GPIOF ISR code:
	GPIOF->DATA &= 0x00;
	if ((GPIOF->MIS & 0x10) == 0x10){
		//If SW1 is pressed, PF4 interrupts and red LED is on for a few secs
		GPIOF->DATA |= 0x02;
		Delay(1000);
	}
	else if ((GPIOF->MIS & 0x01) == 0x01){
		//If SW2 is pressed, PF0 interrupts and white LED blinks 4 times for a few secs
		for (int i = 0; i < 8; i++){
			GPIOF->DATA ^= 0x0E;
			Delay(800);
		}
	}
	GPIOF->DATA &= 0x00; //Clear Port F
	GPIOF->ICR |= 0x11;
}

void Delay(unsigned long delayMs){
	unsigned long i;
	for (i = 0; i < delayMs * 11416; i++){}
}
