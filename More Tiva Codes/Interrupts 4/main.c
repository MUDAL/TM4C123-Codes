/*
	Interrupts program 4 (Naive code)
	Description::
	If Switch is pressed, blue LED blinks
	If Switch is pressed again, LED goes off
	
	Issue:: Button bounce (solved with hardware debouncing)
*/

#include "TM4C123.h"                    // Device header
void Delay(unsigned long delayMs);
int flag = 0;

int main(){
	SYSCTL->RCGCGPIO |= 0x28;
	
	GPIOD->DIR &= ~0x01; //make PD0 an input pin
	GPIOF->DIR |= 0x0E;
	GPIOD->DEN |= 0x01;
	GPIOF->DEN |= 0x0E;

	__disable_irq();
	GPIOD->IS &= ~0x01; //make PD0 edge sensitive
	GPIOD->IBE &= ~0x01; //not enabling interrupts for both edges
	GPIOD->IEV &= ~0x01; //falling edge detection
	GPIOD->ICR |= 0x01; //clear any prior interrupt
	GPIOD->IM |= 0x01; //unmask interrupt
	
	NVIC->IP[3] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x08; //enable IRQ3
	
	__enable_irq();
	
	while (1){
		if (flag){
			GPIOF->DATA ^= 0x04;
			Delay(400);
		}
	}
}

void GPIOD_Handler(void){
	//GPIOD ISR code:
	if ((GPIOD->MIS & 0x01) == 0x01){
		if (!flag){
			flag = 1; //blue LED blinks
		}
		else{
			flag = 0;
			GPIOF->DATA &= 0x00; //LED goes off
		}
	}
	GPIOD->ICR |= 0x01;
}


void Delay(unsigned long delayMs){
	unsigned long i;
	for (i = 0; i < delayMs * 11416; i++){}
}
