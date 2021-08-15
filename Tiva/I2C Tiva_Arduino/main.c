#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "BUTTON.h"
#include "I2C.h"
#include "TIMER.h"

/*
APPROVED:: 04-07-2020
*/

int main(){
	
	Button_t button1 = {0,0};
	Button_t button2 = {0,0};
	
	Button_Init();
	I2C_Init();
	Timer_Init();
	
	while(1){
		button1.pressed = !(GPIO_PORTF_DATA_R & BUTTON1);
		button2.pressed = !(GPIO_PORTF_DATA_R & BUTTON2);
		
		if (Button_Debounce(button1.pressed) && !button1.prevPressed){
			I2C_sendAddress(GENERAL_CALL_ADDRESS);
			I2C_sendData('5');
			button1.prevPressed = 1;
		}
		else if (!Button_Debounce(button1.pressed) && button1.prevPressed){
			button1.prevPressed = 0;
		}
		
		if (Button_Debounce(button2.pressed) && !button2.prevPressed){
			I2C_sendAddress(GENERAL_CALL_ADDRESS);
			I2C_sendData('9');
			button2.prevPressed = 1;
		}
		else if (!Button_Debounce(button2.pressed) && button2.prevPressed){
			button2.prevPressed = 0;
		}
		
	}
	
}

