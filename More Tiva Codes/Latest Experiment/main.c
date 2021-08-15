//TIMER CONFIGURATION:: INPUT CAPTURE MODE

/*
*	We'll continue from here
*/

#include "TM4C123.h"                    // Device header
#include <stdlib.h>
#include <stdint.h>

volatile int flag1 = 0;
volatile int flag2 = 0;
unsigned long read1 = 0;
unsigned long read2 = 0;
void Timer_Init(void);

int  main(){

	SYSCTL->RCGCGPIO |= 0x22; //Clock for ports F,B
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //Disable system clock divisor
	
	//======= unlock PF0 =======
//	GPIOF->LOCK = 0x4C4F434B;
//	GPIOF->CR = 0x01;
	//==========================
	
	//==== unlock PB ==========
	GPIOB->LOCK = 0x4C4F434B;
	GPIOB->CR = 0x0C;
	//==========================
	
//	GPIOF->DIR &= ~0x01; //make PF0 an input pin
//	GPIOF->DEN |= 0x01; //enable digital functions for PF0
//	GPIOF->AFSEL |= 0x01; //alternate functions for PF0
//	GPIOF->PCTL |= 0x00007; //enable input capture for PF0
//	GPIOF->PUR |= 0x01; //enable pullup for PF0
	
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E; 
	GPIOB->DIR &= ~0x41; //Configure PB0 and PB6 as input
	GPIOB->DEN |= 0x41; //Enables digital functions for PB0 and PB6
	GPIOB->AFSEL |= 0x41; //Enable alternate functions for PB0 and PB6
	GPIOB->PCTL |= 0x07000007; //enable input capture for PB0 and PB6
	
	__disable_irq();
	
	Timer_Init(); //Initialize timer0A and 2A
	TIMER0->CTL |= 0x01; //Enable timer0A to start counting
	TIMER0->SYNC |= 0x11; //synchronize timers 0A and 2A
	TIMER2->CTL |= 0x01; //Enable timer2A to start counting
	
//	GPIOF->IS &= ~0x01; //make PF0 edge sensitive
//	GPIOF->IBE &= ~0x01; //disabling both-edge interrupts
//	GPIOF->IEV &= ~0x01; //falling edge detection
//	GPIOF->ICR |= 0x01; //clear any prior interrupt
//	GPIOF->IM |= 0x01; //unmask interrupt
	
	//====== PORTB INTERRUPTS ======================================
	GPIOB->IS &= ~0x41; //make PB0 and PB6 edge sensitive
	GPIOB->IBE &= ~0x41; //disabling both edge interrupts for PB0 and PB6
	GPIOB->IEV &= ~0x41; //falling edge detection for PB0
	GPIOB->ICR |= 0x41; //clear any prior interrupt
	GPIOB->IM |= 0x41; //unmask interrupt
	NVIC->IP[1] = 1<<5; //Set priority 1 for port B interrupts
	NVIC->ISER[0] = 0x02; //enable IRQ1
	//================================================================
//	NVIC->IP[30] = 1<<5; //set priority 1 (for GPIOF)
//	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	__enable_irq();
	
	while(1){
		
//		if (flag1){
//			if ((TIMER0->RIS & 0x04) == 0x04){//waiting for event
//				TIMER0->ICR |= 0x04; //clear timeout flag
//				read1 = TIMER0->TAR;
//			}
//			flag1 = 0;
//		}
//		if (flag2){
//			if ((TIMER0->RIS & 0x04) == 0x04){//waiting for event
//				TIMER0->ICR |= 0x04; //clear timeout flag
//				read2 = TIMER0->TAR;
//			}
//			flag2 = 0;
//		}
		if (flag1) {
			read1 = TIMER0->TAR;
		}
		if (flag2) {
			read2 = TIMER2->TAR;
		}
		//GPIOF->DATA ^= 0x0E;
	}	
}

void Timer_Init(){
	volatile unsigned long delay;
	SYSCTL->RCGCTIMER |= 0x05; // Timer 0 and Timer 2 clock
	delay = SYSCTL->RCGCTIMER;
	TIMER0->CFG = 0x04; //16 bit timer
	TIMER0->TAMR |= 0x07; //Capture mode, Edge-Time mode
	TIMER0->CTL |= 0x04; //Allow timer to capture negative-edge events
	TIMER0->TAILR = 65536 - 1;
	//	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
 //	NVIC->ISER[0] = 0x80000; //enable IRQ19
//	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
	
	TIMER2->CFG = 0x04; //16 bit timer
	TIMER2->TAMR |= 0x07; //Capture mode, Edge-Time mode
	TIMER2->CTL |= 0x04; //Allow timer to capture negative-edge events
	TIMER2->TAILR = 65536 - 1;

//	TIMER2->IMR |= 0x01; //Enable interrupts for the timer
//	NVIC->ISER[0] = 0x800000; //enable IRQ23
//	NVIC->IP[23] = 2<<5; //set priority 2 for IRQ23
}

//void GPIOF_Handler(void){
//	flag1 = 1;
//	GPIOF->ICR |= 0x01; 
//}

//void TIMER0A_Handler(void){
//	TIMER0->ICR |= 0x04; //clear timeout flag
//}

void GPIOB_Handler(void){
	
	if ((GPIOB->MIS & 0x01) == 0x01){flag1 = 1;}
	if ((GPIOB->MIS & 0x40) == 0x40){flag2 = 1;}
	GPIOB->ICR |= 0x41; 
}

