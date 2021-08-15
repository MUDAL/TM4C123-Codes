#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "Button.h"
#include "SPI.h"
#include "Timer.h"

//uncomment Timer 0 interrupt handler in startup file

int main(){
	
	Button_t button1 = {0,0};
	Button_t button2 = {0,0};
	
	Button_Init();
	SPI_Init();
	Timer_Init();
	
	while(1){
		button1.pressed = !(GPIO_PORTF_DATA_R & BUTTON1);
		button2.pressed = !(GPIO_PORTF_DATA_R & BUTTON2);
		
		if (Button_Debounce(button1.pressed) && !button1.prevPressed){
			SPI_Send('1');
			button1.prevPressed = 1;
		}
		else if (!Button_Debounce(button1.pressed) && button1.prevPressed){
			button1.prevPressed = 0;
		}
		
		if (Button_Debounce(button2.pressed) && !button2.prevPressed){
			SPI_Send('2');
			button2.prevPressed = 1;
		}
		else if (!Button_Debounce(button2.pressed) && button2.prevPressed){
			button2.prevPressed = 0;
		}
		
	}
	
}


