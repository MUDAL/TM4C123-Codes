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
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/*
	* Simple FreeRTOS code to control 
	* brightness of an LED
*/

uint32_t SystemCoreClock = 16000000;
QueueHandle_t myQueue;
void ConfigureUART(void);
void ConfigureADC(void);
void ConfigurePWM(void);

void ADCTask(void *pvParameters){
	
	uint32_t adcValue;
	myQueue = xQueueCreate(5, sizeof(uint32_t));
	
	for(;;){
		while(!MAP_ADCIntStatus(ADC0_BASE, 3, false)){}
//		MAP_ADCIntClear(ADC0_BASE, 3);
		MAP_ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);
			
		if(uxQueueMessagesWaiting(myQueue) == 0){
			xQueueSend(myQueue, &adcValue, pdMS_TO_TICKS(0));
		}
			
	}
	
}

void PWMTask(void *pvParameters){
	
	uint32_t ledBrightness;
	for(;;){
		if(myQueue != 0){
			if(xQueueReceive(myQueue, &ledBrightness, pdMS_TO_TICKS(100))){
				MAP_TimerMatchSet(TIMER0_BASE, TIMER_A, ledBrightness);
			}
		}
	}
	
}

void UARTTask(void *pvParameters){
	
	uint32_t Brightness;
	for(;;){
		if(myQueue != 0){
			if(xQueueReceive(myQueue, &Brightness, pdMS_TO_TICKS(10))){
				UARTprintf("\nbrightness of LED: %d\n",4095 - Brightness);
			}
		}
	}
	
}	

int main(){
	
	BaseType_t task1Status;
	BaseType_t task2Status;
	BaseType_t task3Status;
	
	// Set the clocking to run from the PLL at 50MHz.
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	ConfigureUART();
	ConfigureADC();
	ConfigurePWM();
	
	task1Status = xTaskCreate(ADCTask, "Task 1", 128, NULL, 1, NULL);
	task2Status = xTaskCreate(PWMTask, "Task 2", 128, NULL, 1, NULL);
	task3Status = xTaskCreate(UARTTask, "Task 3", 128, NULL, 1, NULL);
	
	if (task1Status != pdPASS){
		UARTprintf("Task 1 was not created\n");
	}
	
	if (task2Status != pdPASS){
		UARTprintf("Task 2 was not created\n");
	}
	
	if (task3Status != pdPASS){
		UARTprintf("Task 3 was not created\n");
	}
	
	vTaskStartScheduler();
	for(;;);
	
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

void ConfigureADC(void){
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //Clock for GPIOE
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){} //Wait for GPIOE module to be ready	
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); //Configure PE3 for AIN0
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Enable ADC0 module
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)){} //Wait for ADC0 module to be ready
	
	MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 0);
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, (ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0));
	
	MAP_ADCSequenceEnable(ADC0_BASE, 3);
	
}

void ConfigurePWM(void){
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable Peripheral Clocks
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
	MAP_GPIOPinConfigure(GPIO_PB6_T0CCP0);
	MAP_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
	
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
		
	MAP_TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM));
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, 4096);
	MAP_TimerMatchSet(TIMER0_BASE, TIMER_A, 4096); //change this value in the main of the pwm task
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
	
}
