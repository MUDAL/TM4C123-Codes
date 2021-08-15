#include "TM4C123.h"                    // Device header
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//DATE CREATED:: 28-10-2019

/*
* PROCESS::
* 1) Initialize all peripherals
* 2) Setup necessary interrupts
* 3) Apply the following algorithm
*						ALGORITHM::


*/

uint32_t firstReading = 0;
uint32_t secondReading = 0;
volatile uint16_t eventOne = 0;
volatile uint16_t eventTwo = 0;
volatile uint16_t firstTime_Button1 = 0;
volatile uint16_t firstTime_Button2 = 0;
bool start_firstButton = false;
bool start_secondButton = false;

int main(){
	
	//=== CLOCK GATING CONTROL ===//
	SYSCTL->RCGCGPIO |= 0x22; //Clock for ports F,B
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //Disable system clock divider
	//===========================//
	
	//=== PORT F INIT for LED debugging ===//
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	//=======================================
	
	//=== PORT B INIT ======================//
	GPIOB->DIR &= ~0x41; //Configure PB0 and PB6 as input
	GPIOB->DEN |= 0x41; //Enables digital functions for PB0 and PB6
	GPIOB->AFSEL &= ~0x41; //Disable alternate functions for PB0 and PB6
	
	//=== TIMER 0A INIT ===//
	volatile uint32_t delay;
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	delay = SYSCTL->RCGCTIMER; //delay to stabilize clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR &= ~0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000 - 1; 
	
	// INTERRUPTS
	__disable_irq();
	GPIOB->IS &= ~0x41; //make PB0 and PB6 edge sensitive
	GPIOB->IBE &= ~0x41; //disabling both edge interrupts for PB0 and PB6
	GPIOB->IEV &= ~0x41; //falling edge detection for PB0
	GPIOB->ICR |= 0x41; //clear any prior interrupt
	GPIOB->IM |= 0x41; //unmask interrupt
	NVIC->IP[1] = 1<<5; //Set priority 1 for port B interrupts
	NVIC->ISER[0] = 0x02; //enable IRQ1
	
////	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
////	NVIC->ISER[0] = 0x80000; //enable IRQ19
////	NVIC->IP[19] = 1<<5; //set priority 1 for IRQ19
	//=====================//
	__enable_irq();
	//======================================//
	
	while (1){
		
		if (eventOne){
			/*
			**-- If timer is counting, read timer
			*/
			if (firstTime_Button1 > 1 || firstTime_Button2 >= 1){
				if ((TIMER0->CTL & 0x01) != 0 && !start_firstButton){
					firstReading = TIMER0->TAR;//read timer
				}
				firstTime_Button1 = 2;
			}
		start_firstButton = true;
		start_secondButton = false;
		eventOne = 0;
		}
		
		if (eventTwo){
			/*
			**-- If timer is counting, read timer
			*/
			if (firstTime_Button2 > 1 || firstTime_Button1 >= 1){
				if ((TIMER0->CTL & 0x01) != 0 && !start_secondButton){
					secondReading = TIMER0->TAR;//read timer
				}
				firstTime_Button2 = 2;
			}
		start_secondButton = true;
		start_firstButton = false;
		eventTwo = 0;
		}
		
		//CODE to do something with the readings
		
		/*SET thresholds for the readings in order
		to determine how they'll affect different types
		of button presses.
		*/
		
	}
	
}

void GPIOB_Handler(void){
	
	//ADD code to detect the first time a button is pressed
	TIMER0->CTL |= 0x01; //start timer
	if ((GPIOB->MIS & 0x01) == 0x01){
		//if ((TIMER0->CTL & 0x01) == 0){ //If timer isn't counting
			//TIMER0->CTL |= 0x01; //start timer
		//}
		firstTime_Button1++;
		eventOne = 1; //set flag
	}
	if ((GPIOB->MIS & 0x40) == 0x40){
		//if ((TIMER0->CTL & 0x01) == 0){ //If timer isn't counting
			//TIMER0->CTL |= 0x01; //start timer
		//}
		firstTime_Button2++;
		eventTwo = 1; //set flag
	}
	GPIOB->ICR |= 0x41; 
}

