/*
* Simple code: Press a switch and an LED turns on 
* SW1 is connected to PF4
* Red LED is connected to PF1
*
*/
#include "TM4C123.h"                    // Device header
//Port Initialization
void PortE_Init(void);
//Globals
unsigned long SW1; //reads from port E

int main(){
	
	PortE_Init();
	while (1){
		SW1 = GPIOE->DATA & 0x08;
		if (SW1 == 0x00){//if SW1 is pressed
			GPIOF->DATA |= 0x02; //turn LED on
		}
		else{
			GPIOF->DATA &= 0x00; //LED off
		}
	}
	
}

void PortE_Init(){
	SYSCTL->RCGCGPIO |= 0x32; //Clock for ports F,B and E
	GPIOF->DIR |= 0x02; //PF1 as output
	GPIOF->AFSEL &= 0x00; //no alternate functions
	GPIOF->DEN |= 0x12; //enable digital functions for PF4 and PF1
	GPIOF->PUR |= 0x10; //enable pullup for PF4
	GPIOE->DIR &= 0x00; //Configure all port B pins as inputs
	GPIOE->DEN |= 0xFF; //Enables digital functions for PE7 to PE0
	GPIOE->AFSEL &= 0x00; //Disable alternate functions
}


