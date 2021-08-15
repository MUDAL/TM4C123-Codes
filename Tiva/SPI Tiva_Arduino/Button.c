#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "Button.h"

uint8_t Button_debounced = 0;

void
Button_Init(void)
{
    //
    // Enable Peripheral Clocks 
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		//
		// Unlock the Port Pin and Set the Commit Bit
		//
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR)   |= GPIO_PIN_0;
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = 0x0;
	
    //
    // Configure the GPIO Pin Mux for PF0
		// for GPIO_PF0
    //
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Configure the GPIO Pin Mux for PF4
		// for GPIO_PF4
    //
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

bool Button_Debounce(uint8_t buttonStatus){
	
	if (buttonStatus){
		
		Button_debounced = 0;
		TimerEnable(TIMER0_BASE,TIMER_BOTH);
		while(!Button_debounced){} //wait for 5ms
			
		if (buttonStatus){
			return true;
		}
	}
	
	return false;
	
}

void Timer0AIntHandler(){
	
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	Button_debounced = 1;
	
}


