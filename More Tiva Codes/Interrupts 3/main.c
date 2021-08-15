/*
	Interrupts program 3 (Naive code)
	Description::
	If one switch is pressed, blue LED blinks
	If another is pressed, blue LED goes off
	
	Issue:: Button bounce (solved with hardware debouncing)
*/

#include "TM4C123.h"                    // Device header
void Delay(unsigned long delayMs);
volatile int flag = 0;

int main(){
	SYSCTL->RCGCGPIO |= 0x28;
	
	GPIOD->DIR &= ~0x03; //make PD1 and PD0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOD->DEN |= 0x03;
	GPIOF->DEN |= 0x0E;
	
	__disable_irq();
	GPIOD->IS &= ~0x03; //make PD1 and PF0 edge sensitive
	GPIOD->IBE &= ~0x03; //not enabling interrupts for both edges
	GPIOD->IEV &= ~0x03; //falling edge detection
	GPIOD->ICR |= 0x03; //clear any prior interrupt
	GPIOD->IM |= 0x03; //unmask interrupt
	
	NVIC->IP[3] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x08; //enable IRQ3
	
	__enable_irq();
	
	while (1){
		if (flag == 1){
			GPIOF->DATA ^= 0x04;
			Delay(800);
		}
	}
}

void GPIOD_Handler(void){
	//GPIOD ISR code:
	if ((GPIOD->MIS & 0x02) == 0x02){
		flag = 1;
	}
	else if ((GPIOD->MIS & 0x01) == 0x01){
		flag = 0;
		GPIOF->DATA &= 0x00; 
	}
	GPIOD->ICR |= 0x03;
}


void Delay(unsigned long delayMs){
	unsigned long i;
	for (i = 0; i < delayMs * 11416; i++){}
}
