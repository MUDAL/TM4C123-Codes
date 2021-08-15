#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "Timer.h"

void Timer_Init(){
	
		//
    // Set Timer0A to monostable mode with 5ms counting time
    // This helps in debouncing the buttons
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //Enable Timer 0 clock
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
		{ 
		}
		TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
		TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerLoadSet(TIMER0_BASE,TIMER_A, TIMER_LOAD);
		IntEnable(INT_TIMER0A_TM4C123);
		IntMasterEnable(); // Enable interrupts to the processor
	
}
