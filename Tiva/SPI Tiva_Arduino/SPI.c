#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "SPI.h"

void
SPI_Init(void)
{
		//
    // Set the clocking to run from the main oscillator at 16MHz.
    //
   SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); 
		//
    // Enable SSI module 0 Clock
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0); 
		//
		// Wait for the SSI0 module to be ready.
		//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0))
	{
	}
    //
    // Enable GPIOA Clock
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); 

    //
    // Configure the GPIO Pin Mux for PA4
		// for SSI0RX
    //
	GPIOPinConfigure(GPIO_PA4_SSI0RX);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_4);

    //
    // Configure the GPIO Pin Mux for PA3
		// for SSI0FSS
    //
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_3);

    //
    // Configure the GPIO Pin Mux for PA5
		// for SSI0TX
    //
	GPIOPinConfigure(GPIO_PA5_SSI0TX);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5);

    //
    // Configure the GPIO Pin Mux for PA2
		// for SSI0CLK
    //
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2);
		
		//
    // Enable pullup resistors for all SSI module 0
		// pins and set drive strength to 2mA
    //
	GPIOPadConfigSet(GPIO_PORTA_BASE, (GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5) ,
																	GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_STD_WPU );
	
		//
		// Set SSI Clock Source.
		//
	SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM);
		//
		// Configure the SSI [SPI mode,master,8 bit data frame].
		//
	SSIConfigSetExpClk(SSI0_BASE, SYSTEM_CLOCK, SSI_FRF_MOTO_MODE_0 ,SSI_MODE_MASTER, BIT_RATE , 8);
		//
		// Enable the SSI 0 module.
		//
	SSIEnable(SSI0_BASE); 
}

void SPI_Send(char data){
	
	while(!(SSI0_SR_R & SSI_SR_TFE)){} //wait for Transmit FIFO to be empty
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0); //Slave select
	SSI0_DR_R = data; //Put data into transmit FIFO
		
}

