#include "TM4C123.h"                    // Device header
volatile unsigned long readValue;

int main(){
	/*
	In order for the ADC module to be used, the PLL must be enabled and programmed to a supported crystal
	frequency in the RCC register (see pg 254).
	*/
	
	//If PIOSC is to be used to clock ADC
	//1. Power up the PLL
	//2. Enable PIOSC in the CS field of ADCCC register
	//3. Disable PLL
	
	//Steps for module initialization
	//1. Enable ADC clock using the RCGCADC register (pg 352)
	//2. Enable clock for appropriate GPIO module via RCGCGPIO register (pg 340)
	//3. Set GPIOAFSEL bits for ADC input pins (pg 671)
	//4. Clear bits in GPIODEN (pg 682) for analog functionality
	//5. Set bits in GPIOAMSEL (pg 687) to disable analog isolation circuit
	//6. If required by application, reconfigure the sample sequencer priorities in the ADCSSPRI register.
	
	SYSCTL->RCC |= 0x4000000; //16.67MHz PLL frequency
	
	volatile unsigned delay;
	SYSCTL->RCGCADC |= 0x01; //Clock for ADC0
	//SYSCTL->RCGC0 |= 0x10000; //clock for ADC0 [RCGC0 also works but is specifically for legacy mode]
	//ADC0->CC |= 0x01; //PIOSC provides 16MHz to ADC
	SYSCTL->RCGCGPIO |= 0x08; //Clock for GPIO port D
	delay = SYSCTL->RCGCGPIO;
	GPIOD->AFSEL |= 0x01; //Enable analog functions for PD0
	GPIOD->DEN &= ~0x01; //Disable digital functions of PD0
	GPIOD->AMSEL |= 0x01; //Disable analog isolation circuit
	ADC0->SSPRI ^= 0x3333; //Reconfiguring SS priorities
	
	//Steps to configure sample sequencer
	//1. Disable SS by clearing ASENn bit in the ADCACTSS register
	//2. Configure trigger for SS in the ADCEMUX register
	//3. Configure input source in the ADCSSMUXn register
	//4. For each sample in the SS, configure sample control bits
	// in the corresponding nibble in the ADCSSCTLn register. When programming
	// the last nibble, set the END bit.
	//5. Enable the SS by setting corresponding ASENn bit in the ADCACTSS register.
	
	
	ADC0->ACTSS &= ~0x08; //Disable SS3
	ADC0->EMUX |= 0x00; //Using processor as a trigger source
	ADC0->SSMUX3 |= 0x07; //Input source to be sampled is AIN7 (i.e PD0)
	ADC0->SSCTL3 |= 0x06; //set END bit and IE bit
	/*Setting IE bit allows an interrupt to be raised at the end
	of a conversion. The end of a conversion is signalled when the INR3 bit
	in the ADCRIS register is set.
	*/
	ADC0->ACTSS |= 0x08; //Enable SS3
	
	while (1){
		//Capturing samples by polling
		//1. Initiating the trigger by setting SS3 bit in the ADCPSSI (i.e begin sampling on SS3)
		//2. Polling the ADCRIS register to note the end of a conversion
		//3. Reading the samples
		//4. Clearing the INR3 bit in the ADCRIS register to capture another sample
		
		ADC0->PSSI |= 0x08;
		if ((ADC0->RIS & 0x08) == 0x08){//If INR3 bit in RIS register is set
			readValue = ADC0->SSFIFO3;
			ADC0->ISC |= 0x08; //Clearing INR3 bit is RIS register
		}
	}
	
}
