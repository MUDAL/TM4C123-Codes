#include "TM4C123.h"                    // Device header
#include <stdint.h>
/*
Date:: 30-01-2020
Developing a code to control an LCD without using library functions.
Necessary information are obtained from device datasheets.
*/
//EXTERNAL CONNECTIONS
// PBn(Microcontroller) -> DBn(LCD) :: n = 0,1,2....7

//PROGRAMMING STEPS
//1.) Initialization of the peripherals to use (GPIOB)
//2.) Declarations


#define FUNCTION_SET_1		0b00110000
#define FUNCTION_SET_2		0b00111000
#define DISPLAY_CONTROL		0b00001110
#define ENTRY_MODE				0b00000110
#define LETTER_H					0b01001000
#define LETTER_I					0b01001001
#define LETTER_E					0x45
#define LETTER_L					0x4c
#define LETTER_O					0x4f
#define LETTER_W					0x57
#define LETTER_R					0x52
#define LETTER_D					0x44
#define SPACEKEY					0x20
#define CLEAR_DISPLAY			0x01

void Port_Init(void);
void send2LCD(uint16_t RS, uint32_t data);
void delay(void);

int main(){
	
	Port_Init();
	
	//CODE for initializing LCD by instructions
	/*
	PLACE CODE HERE
	*/
	
	//8bit data transfer, 1 line mode
	send2LCD(0,FUNCTION_SET_1);
	send2LCD(0,DISPLAY_CONTROL);
	send2LCD(0,ENTRY_MODE);
	
	//Display HELLO WORLD and clear the screen
	send2LCD(1,LETTER_H);
	send2LCD(1,LETTER_E);
	send2LCD(1,LETTER_L);
	send2LCD(1,LETTER_L);
	send2LCD(1,LETTER_O);
	send2LCD(1,SPACEKEY);
	send2LCD(1,LETTER_W);
	send2LCD(1,LETTER_O);
	send2LCD(1,LETTER_R);
	send2LCD(1,LETTER_L);
	send2LCD(1,LETTER_D);
	send2LCD(0,CLEAR_DISPLAY);
	
	while(1){
		
	}
}

void delay(){
	for (int i = 0; i < 2000000; i++){}
}

void Port_Init(){
	//Initializing all Port B pins of the microcontroller (for data transfer with the LCD)
	//PE0 for Register Select (RS:0[command mode], RS:1[data mode])
	//PE1 for the ENABLE pin (to be toggled)
	//Digital output pins
	SYSCTL->RCGCGPIO |= 0x32; //Clock for ports F,B and E
	//==== unlock PB7 ==========
	GPIOB->LOCK = 0x4C4F434B;
	GPIOB->CR = 0x0C;
	//==========================
	GPIOB->DIR |= 0xFF; //Configure all port B pins as outputs
	GPIOB->DEN |= 0xFF; //Enables digital functions for PB7 to PB0
	GPIOB->AFSEL &= 0x00; //Disable alternate functions
	
	GPIOE->DIR |= 0xFF; //Configure all port E pins as outputs
	GPIOE->DEN |= 0xFF; //Enables digital functions for PE7 to PE0
	GPIOE->AFSEL &= 0x00; //Disable alternate functions
}


void send2LCD(uint16_t RS ,uint32_t data){
	GPIOE->DATA = (RS << 0);				//PE0 = RS
	GPIOB->DATA = data; //Write data to LCD's RAM
	//Toggle PE1 to create a pulse for ENABLE pin of LCD
	GPIOE->DATA |= 0x02;
	delay();
	GPIOE->DATA &= 0x01;
	delay();
	
}
