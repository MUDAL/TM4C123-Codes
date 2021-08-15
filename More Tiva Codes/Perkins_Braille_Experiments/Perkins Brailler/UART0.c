#include "TM4C123.h"                    // Device header
#include "UART0.h"

//UART_OutChar
/*Waits for transmit FIFO to be empty
	in order for it to receive data from the data register.
*/
void UART_OutChar(unsigned char data){
	while((UART0->FR & 0x20) != 0x00);
	UART0->DR = data;
}

