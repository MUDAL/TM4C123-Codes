#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "Timer.h"

void Timer0_Init(){
		//
    // Set Timer0A to monostable mode with 5ms counting time
    // This helps in debouncing the buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //Enable Timer 0 clock
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER0_BASE,TIMER_A, TIMER0_LOAD);
	
}

void Timer1_Init(){
		//
    // Set Timer1A to monostable mode with 100ms counting time
    // This helps in registering multiple button presses within the time frame
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); //Enable Timer 1 clock
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)){}
	TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER1_BASE,TIMER_A, TIMER1_LOAD);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER1A_TM4C123);
	IntMasterEnable();
		
}
