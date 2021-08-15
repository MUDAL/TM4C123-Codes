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
#include "buttons.h"
#include "Timer.h"

volatile uint8_t Buttons_pressTimeElapsed = 0;
volatile uint8_t Timer_counting = 0;

void
Buttons_Init(void)
{
    //
    // Enable Peripheral Clocks 
    //
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

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
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Configure the GPIO Pin Mux for PF4
	// for GPIO_PF4
    //
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

bool Buttons_Debounce(uint8_t buttonStatus){
	
	if (buttonStatus){
		
		TimerEnable(TIMER0_BASE,TIMER_A);
		while(!(TIMER0_RIS_R & TIMER_RIS_TATORIS)){} //poll the timeout flag
		TIMER0_ICR_R |= TIMER_ICR_TATOCINT;
			
		if (buttonStatus){
			return true;
		}
	}
	
	return false;
	
}

void Buttons_Poll(Buttons_t *button, uint8_t *p_buttonRegister){
	
	if (Buttons_Debounce(button->pressed) && !button->prevPressed){
		
		if (!Timer_counting){
			//TimerEnable(TIMER1_BASE,TIMER_BOTH);
			TIMER1_CTL_R |= TIMER_CTL_TAEN;
			Timer_counting = 1;
		}
		*p_buttonRegister |= (1 << button->bitLocation);
		button->prevPressed = 1;
		
	}
	else if(!Buttons_Debounce(button->pressed) && button->prevPressed){
		button->prevPressed = 0;
	}
	
}

void Timer1AIntHandler(){
	//TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	Buttons_pressTimeElapsed = 1;
	Timer_counting = 0;
	TIMER1_ICR_R |= TIMER_ICR_TATOCINT;
}


