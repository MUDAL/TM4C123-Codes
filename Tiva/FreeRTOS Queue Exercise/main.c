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
#include "utils/ustdlib.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/*
* FreeRTOS Queue Management
* 
*/

QueueHandle_t myQueue;

uint32_t SystemCoreClock = 16000000;
void ConfigureUART(void);

void vTask1(void *pvParameters){
	
	char myTxBuff[30];
	myQueue = xQueueCreate(5, sizeof(myTxBuff));
	for (;;){
		
		if(uxQueueMessagesWaiting(myQueue) == 0){
			usprintf(myTxBuff,"message 1");
			xQueueSend(myQueue, myTxBuff, pdMS_TO_TICKS(5));
			
			usprintf(myTxBuff,"message 2");
			xQueueSend(myQueue, myTxBuff, pdMS_TO_TICKS(5));
			
			usprintf(myTxBuff,"message 3");
			xQueueSend(myQueue, myTxBuff, pdMS_TO_TICKS(5));
		}
	}
		
}

void vTask2(void *pvParameters){
	
	char myRxBuff[30];
	for (;;){
		
		if(uxQueueMessagesWaiting(myQueue) != 0){
			if(xQueueReceive(myQueue, myRxBuff, pdMS_TO_TICKS(10))){
				UARTprintf("data received: %s\n",myRxBuff);
				UARTprintf("unread data: %d\n",uxQueueMessagesWaiting(myQueue));
				UARTprintf("available spaces: %d\n",uxQueueSpacesAvailable(myQueue));
			}
			
		}
	}
		
}

int main()
{
	
	BaseType_t task1Status;
	BaseType_t task2Status;
	
	ConfigureUART();
	task1Status = xTaskCreate(vTask1, "Task 1", 128, NULL, 1, NULL);
	task2Status = xTaskCreate(vTask2, "Task 2", 128, NULL, 1, NULL);
	
	if (task1Status != pdPASS){
		UARTprintf("Task 1 was not created\n");
	}
	
	if (task2Status != pdPASS){
		UARTprintf("Task 2 was not created\n");
	}

	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
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
