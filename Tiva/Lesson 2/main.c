//#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
//#include "inc/hw_gpio.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "driverlib/gpio.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "MyGPIO.h"

void delay(void);


int main(){
	char str[20];
	float n = 2.3;
	int a = 1;
	PinoutSet(); //configure GPIOs
	
	while(1){
		GPIO_PORTF_DATA_R ^= (1<<1);
		delay();
		a++;
		sprintf(str,"%f",n);
	}
}

void delay(void){
	int counter = 0;
	while(counter < 1000000){//delay loop
		++counter;
	}
}
