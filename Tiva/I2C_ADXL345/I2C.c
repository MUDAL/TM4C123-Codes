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
#include "driverlib/i2c.h"
#include "I2C.h"

void I2C_Init(){
	
		//
    // Set the clocking to run from the main oscillator at 16MHz.
    //
  SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); 
		//
    // Enable I2C module 1 Clock
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); 
		//
		// Wait for the I2C1 module to be ready.
		//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
	{
	}
		//
		// Initialize I2C module 1 Master block and 
		// enable speed of 100kbps
		//
	I2CMasterInitExpClk(I2C1_BASE,SYSTEM_CLOCK,false);
		
		//
    // Enable GPIOA Clock
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
    //
    // Configure the GPIO Pin Mux for PA6
		// for I2C1SCL
    //
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);

    //
    // Configure the GPIO Pin Mux for PA7
		// for I2C1SDA
    //
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
		//
    // Enable PA7(I2C1SDA) for open-drain operation
    //
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_OD);

}
