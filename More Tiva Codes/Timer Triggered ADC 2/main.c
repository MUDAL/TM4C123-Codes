/*
This program uses a timer to trigger an ADC.
*/

#include "TM4C123.h"                    // Device header
volatile unsigned long readValue;

//*******Setups**********
void ADC_Init(void);
void Timer_Init(void);
//***********************

int main(){
	
	SYSCTL->RCC |= 0x4000000; //16.67MHz PLL frequency
	__disable_irq();
	ADC_Init();
	Timer_Init();
	__enable_irq();
	
	while (1){}
}

void TIMER0A_Handler(){
	TIMER0->ICR |= 0x01;
}

void ADC0SS3_Handler(){
			readValue = ADC0->SSFIFO3;
			ADC0->ISC |= 0x08; //Clearing INR3 bit is RIS register	
}

void ADC_Init(){
	volatile unsigned long delay;
	SYSCTL->RCGCADC |= 0x01; //Clock for ADC0
	SYSCTL->RCGCGPIO |= 0x08; //Clock for GPIO port D
	delay = SYSCTL->RCGCGPIO;
	GPIOD->AFSEL |= 0x01; //Enable analog functions for PD0
	GPIOD->DEN &= ~0x01; //Disable digital functions of PD0
	GPIOD->AMSEL |= 0x01; //Disable analog isolation circuit
	ADC0->SSPRI ^= 0x3333; //Reconfiguring SS priorities
	
	ADC0->ACTSS &= ~0x08; //Disable SS3
	ADC0->EMUX |= 0x5000; //Using Timer as a trigger source
	ADC0->SSMUX3 |= 0x07; //Input source to be sampled is AIN7 (i.e PD0)
	ADC0->SSCTL3 |= 0x06; //set END bit and IE bit
	/*Setting IE bit allows an interrupt to be raised at the end
	of a conversion. The end of a conversion is signalled when the INR3 bit
	in the ADCRIS register is set.
	*/
	ADC0->IM |= 0x08; //Set MASK3 to send interrupts to the interrupts controller (NVIC)
	ADC0->ACTSS |= 0x08; //Enable SS3
	NVIC->ISER[0] = 0x20000; //enable IRQ17
	NVIC->IP[17] = 3<<5; //set priority 3 for IRQ17
}

void Timer_Init(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x2; //Periodic Timer Mode
  TIMER0->TAMR |= 0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000; // 16kHz --> 1ms
  TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	TIMER0->CTL |= 0x20; //Enable timer to initialize ADC
	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
}
