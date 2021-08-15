#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "basic_UART.h"
#define SYSTEM_CLOCK 20000000

int main(){
	
	__disable_irq();
	char data[6] = "Sake ";
	uint8_t i = 0;
	
	SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_SYSDIV_10|SYSCTL_XTAL_16MHZ); //setup 20MHz clock [PLL,clock divider = 10,16MHz crystal]
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//Enable UART0 clock
	PinoutSet(); //Pin Mux configuration
	
	//System clock = 20MHz, Baud rate = 115200, 8-bit data format, no parity bit
	UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);//UART baud clock source
	UARTConfigSetExpClk(UART0_BASE,SYSTEM_CLOCK,115200,UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
	
	/*UART0->CTL |= (1<<4); //Set EOT bit
	UART0->IM |= (1<<5); //Unmask TX Interrupt
	NVIC->ISER[0] = 0x20; //Enable IRQ19 (UART0)
	UART0->DR = '\0';*/
	__enable_irq();
	
	while(1){
		while((UART0_FR_R & (1<<5)) == (1<<5)); //wait until transmit FIFO is empty
		UART0_DR_R = data[i];
		i++;
		i %= 5;
	}
}

/*void UART0_Handler(){
	static char data[6] = "sail ";
	static uint8_t i = 0;
	UART0->DR = data[i];
	i++;
	i %= 5;
	UART0->ICR = (1<<5); //clear TX interrupt
}*/

