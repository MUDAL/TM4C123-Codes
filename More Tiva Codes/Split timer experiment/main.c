//USING MULTIPLE TIMER MODULES
//PROJECT NAME MIGHT BE MISLEADING

#include "TM4C123.h"                    // Device header

int main(){
	
	//PORT F CONFIGURATION ===========================
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //System clock undivided
	
	//unlock PF0
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	//=================================================
	
	//TIMER CONFIGURATIONS ===================================
	SYSCTL->RCGCTIMER |= 0x03; // Timer 0 and Timer 1 
	//TIMER BLOCK 0
	TIMER0->CFG = 0x0; //32 bit timer configuration
	TIMER0->TAMR |= 0x2; //Periodic Timer Mode for A
  TIMER0->TAILR = 0xFFFF; 
	TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	
	//TIMER BLOCK 1
	TIMER1->CFG = 0x0; //32 bit timer configuration
	TIMER1->TAMR |= 0x1; //One shot Timer Mode for A
  TIMER1->TAILR = 48000000; 
	//========================================================
	
	while(1){

	/*	TIMER1->CTL |= 0x01; //Start Timer
			
		//TIMING CODE
		while ((TIMER1->RIS & 0x01) == 0){
			GPIOF->DATA |= 0x02;
		}
		GPIOF->DATA &= 0x00;*/
		GPIOF->DATA |= 0x02;
		for (int i = 0; i < 1000000; i++){}
		GPIOF->DATA &= 0x00;
		for (int i = 0; i < 1000000; i++){}
	}
	
}

