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
* SUSPENDING AND RESUMING TASK
* 
*/
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

uint32_t SystemCoreClock = 16000000;
void ConfigureUART(void);

void vTask1(void *pvParameters){

	TickType_t previousWakeTime;
	previousWakeTime = xTaskGetTickCount();
	int count = 0;
	for (;;){
		UARTprintf("hello! : %d\n",count++);
		vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(1000));
	}
		
}

void vTask2(void *pvParameters){
	
	TickType_t previousWakeTime;
	previousWakeTime = xTaskGetTickCount();
	for (;;){
		vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(5000));
		vTaskSuspend(task1Handle);
		vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(5000));
		vTaskResume(task1Handle);
	}
		
}

int main()
{
	ConfigureUART();
	xTaskCreate(vTask1, "Task 1", 128, NULL, 1, &task1Handle);
	xTaskCreate(vTask2, "Task 2", 128, NULL, 1, &task2Handle);

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
