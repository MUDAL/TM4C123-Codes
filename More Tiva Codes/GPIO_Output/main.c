/*
*	Simple blinking LED program using 
*	Udemy online course
*/

#include "TM4C123.h"                    // Device header

//Port Initialization and Pseudodelay function
void PortF_Init(void);
void Delay(void);
//void delay(void);

int main(){
	
	PortF_Init();
	while (1){
		GPIOF->DATA ^= 0x0E; //Toggle white LED
		Delay();
	}
}

void PortF_Init(){
	SYSCTL->RCGCGPIO |= 0x20; //Enable clock for port F
	GPIOF->AFSEL &= 0x00; //No alternate function
	GPIOF->DIR |= 0x0E; //PF3-PF1 as outputs
	GPIOF->DEN |= 0x0E; //Enable digital functions for PF3-PF1
}

/*void delay(){
	volatile uint8_t j = 0;
	while (j < 5){
		j++;
	}
}*/

void Delay(){
	volatile uint32_t i = 0;
	while (i < 2000000){
		i++;
	}
	//delay();
}

