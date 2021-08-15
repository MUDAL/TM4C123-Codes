/*
Tiva Mini project 1.0
-> 3 LEDs connected to PE1 - PE3
-> Potentiometer connected to PD0

Connections::
PE1 -> Green LED
PE2 -> Yellow LED
PE3 -> Red LED
PD0 -> Potentiometer

This code can be improved, i'm thinking putting the swicth-case block
into the main loop will be better so as to prevent the ADC handler from
taking too much time executing

*/

#include "TM4C123.h"                    // Device header
volatile unsigned long readValue;

//*******Setups**********
void ADC_Init(void);
void PortE_Init(void);
//***********************

int main(){
	
	SYSCTL->RCC |= 0x4000000; //16.67MHz PLL frequency
	__disable_irq();
	
	ADC_Init();
	PortE_Init();
	
	NVIC->IP[17] = 3<<5; //set priority = 3 for IRQ17
	NVIC->ISER[0] = 0x20000; //enable IRQ17
	
	__enable_irq();
	
	while (1){
		ADC0->PSSI |= 0x08; //Initiate sampling
		
		switch (readValue){
				case 1024:
					//PE1
					GPIOE->DATA = 0x02; //Green
					break;
				case 2048:
					//PE2
					GPIOE->DATA = 0x04; //Yellow
					break;
				case 3072:
					//PE3
					GPIOE->DATA = 0x08; //Red
					break;
				case 300:
				case 4000:
					GPIOE->DATA = 0x00; //All LEDs off
					break;
			}
	}
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
	ADC0->EMUX |= 0x00; //Using processor as a trigger source
	ADC0->SSMUX3 |= 0x07; //Input source to be sampled is AIN7 (i.e PD0)
	ADC0->SSCTL3 |= 0x06; //set END bit and IE bit
	/*Setting IE bit allows an interrupt to be raised at the end
	of a conversion. The end of a conversion is signalled when the INR3 bit
	in the ADCRIS register is set.
	*/
	ADC0->IM |= 0x08; //Set MASK3 to send interrupts to the interrupts controller (NVIC)
	ADC0->ACTSS |= 0x08; //Enable SS3
	
}
	
void PortE_Init(){
	SYSCTL->RCGCGPIO |= 0x10; //Enable clock for port E
	GPIOE->AFSEL &= 0x00; //No alternate function
	GPIOE->DIR |= 0x0E; //PE3-PE1 as outputs
	GPIOE->DEN |= 0x0E; //Enable digital functions for PE3-PE1
}

