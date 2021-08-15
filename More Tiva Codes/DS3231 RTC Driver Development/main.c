#include "TM4C123.h"                    // Device header
#include <stdint.h>

//DATE:: 06-02-2020
//Driver development for DS3231 Real-Time Clock Module
//I2C communication between the DS3231 module and the Tiva C microcontroller

//STEPS
//1.) I2C configuration
//2.) Register and Command Definitions

//INTERNAL REGISTER ADDRESSES
#define SLAVE_ADDRESS 		0x68
#define SECONDS						0x00
#define MINUTES 					0x01
#define HOURS							0x02
#define DAY								0x03
#define DATE							0x04
#define MONTH							0x05
#define YEAR							0x06
//I2C command sequences
#define READ_MODE					0x01
#define WRITE_MODE				0x00
#define START							0x03
#define STOP							0x04

void set_slaveAddress(uint16_t address, uint16_t mode);
void sendByte(uint16_t byte, uint16_t bus_condition);
void I2C_Init(void);

void testDelay(void);

int main(){
	
	I2C_Init();
	//REPEATED START condition
	/*
	1.) send START condition
	2.) send slave address with RW = 0
	3.) send internal register address
	4.) send START sequence again
	5.) send slave address with RW = 1
	6.) Read data byte (read from the I2CMDR)
	7.) send STOP condition
	*/
	//==========================================
	while(1){
		set_slaveAddress(SLAVE_ADDRESS,WRITE_MODE);
		sendByte(MINUTES,START);
		testDelay();
		set_slaveAddress(SLAVE_ADDRESS,READ_MODE);
		I2C0->MCS = START;
		testDelay();
		I2C0->MCS = STOP;
	}
}

void I2C_Init(){
	//SYSTEM CLOCK = 20MHz
	SYSCTL->RCC &= ~0x7000000; 
	SYSCTL->RCC |= 0x4000000;
	//=======================
	SYSCTL->RCGCI2C |= 0x01;//clock for I2C0
	SYSCTL->RCGCGPIO |= 0x02; //clock for port B
	//PB2->SCL(don't configure as open drain) PB3->SDA (open drain)
	GPIOB->AFSEL |= 0x0C; //alternate function for PB2 and PB3
	GPIOB->ODR |= 0x08; //PB3 as open drain
	GPIOB->DEN |= 0x0C; //digital function for PB2 and PB3
	//GPIOB->PCTL |= 0x3300; //assign I2C signals to PB2 and PB3
	I2C0->MCR = 0x10; //enable I2C0 Master mode
	I2C0->MTPR = 0x09; //SCL clock speed = 100kbps
}

void set_slaveAddress(uint16_t address, uint16_t mode){
	I2C0->MSA = (address << 1); //set slave address
	I2C0->MSA |= (mode << 0);
}

void sendByte(uint16_t byte, uint16_t bus_condition){
	I2C0->MDR = byte; //data or internal register address
	while ((I2C0->MCS & 0x01) != 0){} //wait for BUSY bit to be 0
	I2C0->MCS = bus_condition;
}

void testDelay(void){
	for (int i = 0; i < 1000; i++){}
}

