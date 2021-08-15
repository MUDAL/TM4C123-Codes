#include "TM4C123.h"                    // Device header
#include "Serial.h"

//UART_OutChar
/*Waits for transmit FIFO to be empty
	in order to receive data from the data register.
*/
//Input: letter is an 8-bit ASCII character to be transferred
//Output: none
void UART_OutChar(unsigned char data){
	while((UART0->FR & 0x20) != 0x00);
	UART0->DR = data;
}


