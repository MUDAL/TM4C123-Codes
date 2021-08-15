#include "TM4C123.h"                    // Device header
#include <stdlib.h>
#include <string.h>

char buttonPressed[6] = "00000";
unsigned long SW1;
unsigned long SW2;
unsigned long SW3;
int scanButton(char* detect_button);
void UART_OutChar(unsigned char data);
unsigned long read1;
unsigned long read2;

int main(){
	
	int buttonNumber = 0;
	
	SYSCTL->RCGCGPIO |= 0x22; //clock for port F and B
	SYSCTL->RCGCUART |= 0x01; //Clock for UART0
	SYSCTL->RCGCGPIO |= 0x01; //Clock for Port A
	//20MHz PLL frequency
	SYSCTL->RCC &= ~0x7000000; 
	SYSCTL->RCC |= 0x4000000;
	
	GPIOF->DIR |= 0x0E; //PF1-3 as output
	GPIOF->AFSEL &= 0x00; //no alternate functions
	GPIOF->DEN |= 0xE; //enable digital functions for PF1-3
	
	GPIOB->DIR &= 0x00; //all are inputs
	GPIOB->AFSEL &= 0x00; //no alternate functions
	GPIOB->DEN |= 0x07; //enable digital funtions
	
	//====== PORTB INTERRUPTS ======================================
	__disable_irq();
	//====== TIMER CONFIG ==========
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 1600000 - 1; 
	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
	//=============================================================
	
	GPIOB->IS &= ~0x07; //make PB0-2 edge sensitive
	GPIOB->IBE &= ~0x07; //disabling both edge interrupts for PB0-2
	GPIOB->IEV &= ~0x07; //falling edge detection for PB0-2
	GPIOB->ICR |= 0x07; //clear any prior interrupt
	GPIOB->IM |= 0x07; //unmask interrupt
	
	NVIC->IP[1] = 1<<5; //Set priority 1 for port B interrupts
	NVIC->ISER[0] = 0x02; //enable IRQ1
	__enable_irq();
	//=================================================================
	
	//===== UART0 CONFIG ============================================
	GPIOA->DEN |= 0x03; //Enable digital functions for PA0 and PA1
	GPIOA->AMSEL &= ~0x03; //Disable analog functions for PA0 and PA1
	GPIOA->AFSEL |= 0x03; //Enable alternate functions for PA0 and PA1
	GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00)+0x00000011; //Select UART function
	
	//BRD(baud-rate divisor = 10.8507) and baud rate of 115200
	UART0->CTL &= ~0x01; //Disable UART before use
	UART0->IBRD |= 0x0A; //Integer part of 10 (from datasheet)
	UART0->FBRD |= 0x36; //Fractional part of 54 (from datasheet)
	UART0->LCRH |= 0x00000070; //8 bit word length and Enable FIFOs
	UART0->CTL |= 0x01; //Enable UART for use
	//==============================================================
	
	while(1){
//		SW1 = GPIOB->DATA & 0x01;
//		SW2 = GPIOB->DATA & 0x02;
//		SW3 = GPIOB->DATA & 0x04;
//		
//		if (SW1 == 0x00){
//			buttonPressed[0] = '1';
//			//GPIOF->DATA = 0x02;
//		}
//		if (SW2 == 0x00){
//			buttonPressed[1] = '2';
//			//GPIOF->DATA = 0x04;
//		}
//		if (SW3 == 0x00){
//			buttonPressed[2] = '3';
//			//GPIOF->DATA = 0x0E;
//		}
		
//		buttonNumber = scanButton(buttonPressed);
//		switch(buttonNumber){
//			case 1:
//				//do what one button press should do
//				if (!strcmp(buttonPressed,"10000")){
//					//GPIOF->DATA = 0x02;
//					UART_OutChar('a');
//				}
//				else if (!strcmp(buttonPressed,"02000")){
//					//GPIOF->DATA = 0x04;
//					UART_OutChar('b');
//				}
//				else if (!strcmp(buttonPressed,"00300")){
//					//GPIOF->DATA = 0x0E;
//					UART_OutChar('c');
//				}
//			strcpy(buttonPressed,"00000");
//				break;
//			case 2:
//				//do what two button presses should do
//				if (!strcmp(buttonPressed,"12000")){
//					//GPIOF->DATA = 0x02;
//					UART_OutChar('d');
//				}
//				else if (!strcmp(buttonPressed,"02300")){
//					//GPIOF->DATA = 0x04;
//					UART_OutChar('e');
//				}
//				else if (!strcmp(buttonPressed,"10300")){
//					//GPIOF->DATA = 0x0E;
//					UART_OutChar('f');
//				}
//				strcpy(buttonPressed,"00000");
//				break;
//			//case 3:
//				//do what three button presses should do
//				//break;
//		}
		
	}
	
}

void TIMER0A_Handler(){
//	if (buttonPressed[0] == '1') buttonPressed[0] = '0';
//	else if (buttonPressed[1] == '2') buttonPressed[1] = '0';
//	else if (buttonPressed[2] == '3') buttonPressed[2] = '0';
	TIMER0->ICR |= 0x01;
}

void GPIOB_Handler(void){
	//GPIOB ISR code:
	TIMER0->CTL |= 0x01; //Initiate timer
	if ((GPIOB->MIS & 0x01) == 0x01){
		//buttonPressed[0] = '1';
		read1 = TIMER0->TAR;
	}
	else if ((GPIOB->MIS & 0x02) == 0x02){
		//buttonPressed[1] = '2';
		read2 = TIMER0->TAR;
	}
	/*else if ((GPIOB->MIS & 0x04) == 0x04){
		//buttonPressed[2] = '3';
	}*/
	GPIOB->ICR |= 0x07;
}

int scanButton(char* detect_button){
    int count = 0;
    for (int i = 0; i < 5; i++){
        if (detect_button[i] != '0'){
            count++;
        }
    }
   return count;
}

void UART_OutChar(unsigned char data){
	while((UART0->FR & 0x20) == 0x20);
	UART0->DR = data;
}

