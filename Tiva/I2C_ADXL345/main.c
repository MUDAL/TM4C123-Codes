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

/*
APPROVED
*/

uint32_t adxl345 = 0;

int main(){
	
	/*
	* The sequence below is a multi-byte master transmit
	* Two data bytes are transmitted and they are:
	* 1. The address of POWER_CTL register of the ADXL345	
	* 2. The data to be put into the register
	* The sequence follows the flowchart and other info from the..
	* datasheet of the Tm4c123 and the ADXL345
	*/
	
	I2C_Init(); //Initialize Tiva LaunchPad as I2C Master devive
	
	I2CMasterSlaveAddrSet(I2C1_BASE,ADXL345_ADDRESS,false); //send slave address + W
	I2CMasterDataPut(I2C1_BASE,POWER_CTL_ADDRESS); //place first data byte in the I2C1MDR register of LaunchPad
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_SEND_START); //START followed by TRANSMIT
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);  //LaunchPad remains in TRANSMIT mode
	while((I2C1_MCS_R & I2C_MCS_BUSY) == I2C_MCS_BUSY){}  //wait until transmit completes
		
	I2CMasterDataPut(I2C1_BASE,MEASURE); //place second data byte in the MDR register of LaunchPad
	I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);  //TRANSMIT followed by STOP
	while((I2C1_MCS_R & I2C_MCS_BUSY) == I2C_MCS_BUSY){}  //wait until transmit completes
		
	for(;;){
		
		I2CMasterSlaveAddrSet(I2C1_BASE,ADXL345_ADDRESS,false); //send slave address + W
		I2CMasterDataPut(I2C1_BASE,POWER_CTL_ADDRESS);  //place first data byte in the I2C1MDR register of LaunchPad
		I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_SEND_START);  //START followed by TRANSMIT
		while((I2C1_MCS_R & I2C_MCS_BUSY) == I2C_MCS_BUSY){}  //wait until transmit completes
		I2CMasterSlaveAddrSet(I2C1_BASE,ADXL345_ADDRESS,true); //send slave address + R
			
		I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_RECEIVE_START);//Repeated START followed by RECEIVE (master remains in RECEIVE mode)
		while((I2C1_MCS_R & I2C_MCS_BUSY) == I2C_MCS_BUSY){}  //wait until receive completes
		adxl345 = I2CMasterDataGet(I2C1_BASE);
		//I2CMasterControl(I2C1_BASE,I2C_MASTER_CMD_BURST_RECEIVE_FINISH);  //RECEIVE followed by STOP
		
	}
	
}
