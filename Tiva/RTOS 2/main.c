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
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include <FreeRTOS.h>
#include <task.h>

/*
*	Basic program to blink two LEDs at the same time:
* One blinks every 1 second
* the other blinks every 2 seconds
*/

//STEPS
//1. Create two tasks: each one for blinking 

uint32_t SystemCoreClock = 16000000;
void ConfigureUART(void);

void vBlinkLED1Task(void *pvParameters){
 
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);// Enable Peripheral Clocks  
//	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))// Check if the peripheral access is enabled.
//	{
//	}
//	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3); //Configure the GPIO Pin Mux for PB3
	TickType_t previousWakeTime;
	previousWakeTime = xTaskGetTickCount();
	int count = 0;
	for (;;){
		
		UARTprintf("Let's begin : %d\n",count++);
		vTaskDelayUntil(&previousWakeTime ,pdMS_TO_TICKS(2000));
		//MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
		//UARTprintf("Task 1 first time\n");
		//vTaskDelay( pdMS_TO_TICKS( 1000 ) ); // place Task1 in blocked state for 1s.
		//MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x0);
		//UARTprintf("Task 1 second time\n");
    //vTaskDelay( pdMS_TO_TICKS( 1000 ) ); // place Task1 in blocked state for 1s.
		
	}
		
}

void vBlinkLED2Task(void *pvParameters){
//	volatile int count = 0;
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);// Enable Peripheral Clocks  
//	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))// Check if the peripheral access is enabled.
//	{
//	}
//	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4); //Configure the GPIO Pin Mux for PB4
	for (;;){
		//count++;
		//MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);  
		UARTprintf("Task 2 first time\n");
		vTaskDelay( pdMS_TO_TICKS( 2000 ) ); // place Task2 in blocked state for 2s.  
		//MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x0);    
		UARTprintf("Task 2 second time\n");
		vTaskDelay( pdMS_TO_TICKS( 2000 ) ); // place Task2 in blocked state for 2s.
	
	}
		
}


int main()
{
	ConfigureUART();
	xTaskCreate(vBlinkLED1Task, "Task 1", 128, NULL, 1, NULL);
	//xTaskCreate(vBlinkLED2Task, "Task 2", 128, NULL, 1, NULL);

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	
	// The following line should never be reached.  Failure to allocate enough
	//	memory from the heap would be one reason.
	for (;;);
	
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
    UARTStdioConfig(0, 115200, SystemCoreClock);
}
