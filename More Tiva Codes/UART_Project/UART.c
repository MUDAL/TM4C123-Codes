#include "TM4C123.h"                    // Device header
#include "UART.h"

//UART_Init
//UART initialization and configuration
void UART_Init(){
	//U0Rx and U0Tx (PA0 and PA1)
	SYSCTL->RCGCUART |= 0x01; //Clock for UART0
	SYSCTL->RCGCGPIO |= 0x01; //Clock for Port A
	GPIOA->DEN |= 0x03; //Enable digital functions for PA0 and PA1
	GPIOA->AMSEL &= ~0x03; //Disable analog functions for PA0 and PA1
	GPIOA->AFSEL |= 0x03; //Enable alternate functions for PA0 and PA1
	GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011; //Select UART function
	
	//BRD(baud-rate divisor = 10.8507) and baud rate of 115200
	UART0->CTL &= ~0x01; //Disable UART before use
	UART0->IBRD |= 0x0A; //Integer part of 10 (from datasheet)
	UART0->FBRD |= 0x36; //Fractional part of 54 (from datasheet)
	UART0->LCRH |= 0x00000070; //8 bit word length and Enable FIFOs
	//UART0->CC &= 0x00; //Use MOSC to drive PLL for UART0
}

//UART_InChar
//Wait for receive FIFO to be non-empty
//Input: none
//Output: ASCII code for key typed
unsigned char UART_InChar(void){
	while((UART0->FR & 0x10) == 0x10);
	return ((unsigned char)(UART0->DR & 0xFF));
}

//UART_OutChar
//Wait for transmit FIFO to be empty
//Input: letter is an 8-bit ASCII character to be transferred
//Output: none
void UART_OutChar(unsigned char data){
	while((UART0->FR & 0x20) == 0x20);
	UART0->DR = data;
}

//UART_OutString
void UART_OutString(char* string){
	int i = 0;
	while (*(string + i) != '\0'){
		UART_OutChar(*(string + i));
		i++;
	}
}
//UART_InString
//Reads input string and displays it
void UART_InString(char* readStr){
	int i;
	char readChar;
	for (i = 0; i < MAX; i++){
		
			readChar = UART_InChar();
			UART_OutChar(readChar);
			readStr[i] = readChar;
		
			if (readChar == '\r'){
				break;
			}
		}
		readStr[i] = '\0';
		UART_OutString("\n\r");
}

