#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

uint32_t adcValue = 0;

int main(){
	
	float batteryVoltage = 0;
	char buffer[6];
	// Set the clocking to run from the PLL at 50MHz.
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	//MAP_FPULazyStackingEnable();
	//MAP_FPUHalfPrecisionModeSet(FPU_HALF_IEEE);
	//MAP_FPURoundingModeSet(FPU_ROUND_NEAREST);
	//MAP_FPUEnable(); 
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, 50000000);
	MAP_TimerControlTrigger(TIMER0_BASE,TIMER_A,true);
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //Clock for GPIOE
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){} //Wait for GPIOE module to be ready	
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); //Configure PE3 for AIN0
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Enable ADC0 module
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){} //Wait for ADC0 module to be ready
	//MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 0);
	MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, (ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0));
	
	MAP_ADCSequenceEnable(ADC0_BASE, 3);	
	//MAP_ADCIntEnable(ADC0_BASE, 3);
	//MAP_IntEnable(INT_ADC0SS3_TM4C123);
	//MAP_IntMasterEnable();
	
	while(1){
		
		while(!MAP_ADCIntStatus(ADC0_BASE, 3, false)){}
		MAP_ADCIntClear(ADC0_BASE, 3);
		MAP_ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);
		batteryVoltage = adcValue * 5.83 / 1000;
		sprintf(buffer, "%.1f", batteryVoltage);
			
	}
}

//void ADC0SS3IntHandler(){
//	MAP_ADCIntClear(ADC0_BASE, 3);
//	MAP_ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);
//}
