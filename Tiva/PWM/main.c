#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

void ConfigureUART(void);

int main(){
	//66% duty cycle [16-bit example in the datasheet]
	// Set the clocking to run from the PLL at 50MHz.
	//The load value determines the period/frequency of the PWM signal
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
		
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable Peripheral Clocks
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
	MAP_GPIOPinConfigure(GPIO_PB6_T0CCP0);
	MAP_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
		
	MAP_TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, 0xC350); //use 4096 [for adc_pwm rtos]
	MAP_TimerMatchSet(TIMER0_BASE, TIMER_A, 0x411A); //use potentiometer value [for adc_pwm rtos]
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
	
	ConfigureUART();

	for(;;){
	}
}

void
ConfigureUART(void)
{
	/*
	Configure the UART and its pins.  This must be called before UARTprintf().
	*/
	
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    MAP_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, MAP_SysCtlClockGet());
}

