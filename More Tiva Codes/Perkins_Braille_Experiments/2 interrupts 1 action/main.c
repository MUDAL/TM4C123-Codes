#include "TM4C123.h"                    // Device header
#include <stdlib.h>
#include <stdint.h>

volatile uint8_t buttons = 0b00000000;
volatile uint8_t check_buttons = 0;
volatile uint8_t doneCounting = 1;

void Timer_Init(void);

int main(){
	
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	SYSCTL->RCGCGPIO |= 0x10; //Enable clock for port E
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //Disable system clock divisor
	
	//======= unlock PF0 =======
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	//==========================
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	
	__disable_irq();
	
	Timer_Init(); //Initialize timer0A
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 1<<5; //set priority 1 (for GPIOF)
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	__enable_irq();
	
	while(1){
		
		if (check_buttons){
			
			switch (buttons){
				case 0b00000001:
					GPIOF->DATA = 0x04; //turn LED 1 on if button 1 is pressed
					break;
				case 0b00000011:
					GPIOF->DATA = 0x02; //turn LED 2 on if both buttons are pressed
					break;
				case 0b00000010:
					GPIOF->DATA = 0x0E; //turn LED 3 on if button 2 is pressed
					break;
				
			}
			buttons &= 0b00000000;
			check_buttons = 0;
		}
	}
}

void TIMER0A_Handler(){
	doneCounting = 1;
	check_buttons = 1;
	TIMER0->ICR |= 0x01;
}

void GPIOF_Handler(void){
	if (doneCounting){//If timer isn't counting
		TIMER0->CTL |= 0x01; //Enable the timer  to start counting
		doneCounting = 0;
	}
	if ((GPIOF->MIS &0x10) == 0x10) { //if button 1 raises an interrupt
		buttons |= (1<<0);
	}
	else if ((GPIOF->MIS &0x01) == 0x01) { //if button 2 raises an interrupt
		buttons |= (1<<1);
	}
	GPIOF->ICR |= 0x11; //clear interrupt flag
}

void Timer_Init(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 1600000 - 1; 
	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
}

