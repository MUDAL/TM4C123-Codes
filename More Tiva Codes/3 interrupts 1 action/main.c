
#include "TM4C123.h"                    // Device header
void mainInit(void);
void timerInit(void);

struct timeParams {
	unsigned long start;
	unsigned long stop1;
	unsigned long stop2;
}time;

int main(){
	mainInit();
	timerInit();
	
	while (1){
	}
}

void mainInit(){
	SYSCTL->RCGCGPIO |= 0x28; //clock for ports D and F
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
	GPIOD->DIR &= ~0x07; //make PD0-2 input pins
	GPIOF->DIR |= 0x0E; //PF1-3 as outputs
	GPIOD->DEN |= 0x07; //digital functions for PD0-2
	GPIOF->DEN |= 0x0E; //digital functions for PF1-3
	
	__disable_irq();
	
	GPIOD->IS &= ~0x07; //make PD0-2 edge sensitive
	GPIOD->IBE &= ~0x07; //disabling interrupts for both edges of PD0-2
	GPIOD->IEV &= ~0x07; //falling edge detection for PD0-2
	GPIOD->ICR |= 0x07; //clear any prior interrupt
	GPIOD->IM |= 0x07; //unmask interrupt
	
	NVIC->IP[3] = 3<<5; //set priority 3
	NVIC->ISER[0] = 0x08; //enable IRQ3
	__enable_irq();
	
}

void timerInit(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x00; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000 - 1; // 16kHz --> 1ms
	time.start = TIMER0->TAILR;
}

void GPIOD_Handler(void){
	//GPIOD ISR code:
	if ((GPIOD->MIS & 0x02) == 0x02) {
		TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	}
	
	else if ((GPIOD->MIS & 0x01) == 0x01) {
		TIMER0->CTL &= ~0x01; //Disable timer
		time.stop1 = TIMER0->TAV;
		TIMER0->TAV = TIMER0->TAILR;
	}
	
	else if ((GPIOD->MIS & 0x04) == 0x04) {
		TIMER0->CTL &= ~0x01; //Disable timer
		time.stop2 = TIMER0->TAV;
		TIMER0->TAV = TIMER0->TAILR;
	}
	
	GPIOD->ICR |= 0x07; 
}
