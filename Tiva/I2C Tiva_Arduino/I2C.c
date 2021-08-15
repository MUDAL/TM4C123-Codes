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
	
	
  SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
		
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7 , GPIO_STRENGTH_2MA , GPIO_PIN_TYPE_OD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
	{
	}
		
	I2CMasterInitExpClk(I2C1_BASE,SYSTEM_CLOCK,true); //I2C master setup with speed = 400kbps
}

void I2C_sendAddress(uint8_t addr){
	I2C1_MSA_R = addr;
}

void I2C_sendData(char data){
	I2C1_MDR_R = data;
	I2C1_MCS_R = (I2C_MCS_START)|(I2C_MCS_RUN)|(I2C_MCS_STOP); //initiate single byte transmit
	while((I2C1_MCS_R & I2C_MCS_BUSY) == I2C_MCS_BUSY){}//wait until transmit completes
}
