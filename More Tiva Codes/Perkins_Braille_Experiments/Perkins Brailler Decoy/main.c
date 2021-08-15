#include "TM4C123.h"                    // Device header
#include "Config.h"
#include "Serial.h"
#include <string.h>

char buttonPressed[8] = "0000000";
volatile uint16_t timer_notCounting = 1;
volatile uint16_t check_buttonPressed = 0;

void clearArray(char* buttonArray);

int main(){
	
	int uart_variable = 0;
	//unsigned char character;
	Config(); //Initialization function

	while(1){
		/*The main code checks the combination of button presses
		before the timer stopped counting.
		*/
		
		if (check_buttonPressed){
			if (!strcmp(buttonPressed,"1000000")){
				uart_variable = 1;
			}
			
			else if (!strcmp(buttonPressed,"1200000")){
				uart_variable = 2;
			}
			else if (!strcmp(buttonPressed,"1000400")){
				uart_variable = 3;
			}
			else if (!strcmp(buttonPressed,"1000450")){
				uart_variable = 4;
			}
			else if (!strcmp(buttonPressed,"1000050")){
				uart_variable = 5;
			}
			else if (!strcmp(buttonPressed,"1200400")){
				uart_variable = 6;
			}
			else if (!strcmp(buttonPressed,"1200450")){
				uart_variable = 7;
			}
			else if (!strcmp(buttonPressed,"1200050")){
				uart_variable = 8;
			}
			else if (!strcmp(buttonPressed,"0200400")){
				uart_variable = 9;
			}
			else if (!strcmp(buttonPressed,"0200450")){
				uart_variable = 10;
			}
			else if (!strcmp(buttonPressed,"1030000")){
				uart_variable = 11;
			}
			else if (!strcmp(buttonPressed,"1230000")){
				uart_variable = 12;
			}
			else if (!strcmp(buttonPressed,"1030400")){
				uart_variable = 13;
			}
			else if (!strcmp(buttonPressed,"1030450")){
				uart_variable = 14;
			}
			else if (!strcmp(buttonPressed,"1030050")){
				uart_variable = 15;
			}
			else if (!strcmp(buttonPressed,"1230400")){
				uart_variable = 16;
			}
			else if (!strcmp(buttonPressed,"1230450")){
				uart_variable = 17;
			}
			else if (!strcmp(buttonPressed,"1230050")){
				uart_variable = 18;
			}
			else if (!strcmp(buttonPressed,"0230400")){
				uart_variable = 19;
			}
			else if (!strcmp(buttonPressed,"0230450")){
				uart_variable = 20;
			}
			else if (!strcmp(buttonPressed,"1030006")){
				uart_variable = 21;
			}
			else if (!strcmp(buttonPressed,"1230006")){
				uart_variable = 22;
			}
			else if (!strcmp(buttonPressed,"0200456")){
				uart_variable = 23;
			}
			else if (!strcmp(buttonPressed,"1030406")){
				uart_variable = 24;
			}
			else if (!strcmp(buttonPressed,"1030456")){
				uart_variable = 25;
			}
			else if (!strcmp(buttonPressed,"1030056")){
				uart_variable = 26;
			}
		
		switch (uart_variable ){
			case 1:
				UART_OutChar('a');
				clearArray(buttonPressed);
				break;
			
			case 2:
				UART_OutChar('b');
				clearArray(buttonPressed);
				break;
				
			case 3:
				UART_OutChar('c');
				clearArray(buttonPressed);
				break;
				
			case 4:
				UART_OutChar('d');
				clearArray(buttonPressed);
				break;
				
			case 5:
				UART_OutChar('e');
				clearArray(buttonPressed);
				break;
			
			case 6:
				UART_OutChar('f');
				clearArray(buttonPressed);
				break;
			
			case 7:
				UART_OutChar('g');
				clearArray(buttonPressed);
				break;
			
			case 8:
				UART_OutChar('h');
				clearArray(buttonPressed);
				break;
			
			case 9:
				UART_OutChar('i');
				clearArray(buttonPressed);
				break;
			
			case 10:
				UART_OutChar('j');
				clearArray(buttonPressed);
				break;
			
			case 11:
				UART_OutChar('k');
				clearArray(buttonPressed);
				break;
			
			case 12:
				UART_OutChar('l');
				clearArray(buttonPressed);
				break;
			
			case 13:
				UART_OutChar('m');
				clearArray(buttonPressed);
				break;
			
			case 14:
				UART_OutChar('n');
				clearArray(buttonPressed);
				break;
			
			case 15:
				UART_OutChar('o');
				clearArray(buttonPressed);
				break;
			
			case 16:
				UART_OutChar('p');
				clearArray(buttonPressed);
				break;
			
			case 17:
				UART_OutChar('q');
				clearArray(buttonPressed);
				break;
			
			case 18:
				UART_OutChar('r');
				clearArray(buttonPressed);
				break;
			
			case 19:
				UART_OutChar('s');
				clearArray(buttonPressed);
				break;
			
			case 20:
				UART_OutChar('t');
				clearArray(buttonPressed);
				break;
			
			case 21:
				UART_OutChar('u');
				clearArray(buttonPressed);
				break;
			
			case 22:
				UART_OutChar('v');
				clearArray(buttonPressed);
				break;
			
			case 23:
				UART_OutChar('w');
				clearArray(buttonPressed);
				break;
			
			case 24:
				UART_OutChar('x');
				clearArray(buttonPressed);
				break;
			
			case 25:
				UART_OutChar('y');
				clearArray(buttonPressed);
				break;
			
			case 26:
				UART_OutChar('z');
				clearArray(buttonPressed);
				break;
//			default:
//				character = ' ';
			}
			check_buttonPressed = 0;
		}
		
	}
}

void clearArray(char* buttonArray){
	for (int i = 0; i < 7; i++){
        buttonArray[i] = '0';
    }
}


void TIMER0A_Handler(){
	/*This ISR executes when the timer stops counting*/
	timer_notCounting = 1;
	check_buttonPressed = 1;
	TIMER0->ICR |= 0x01;
}

void GPIOB_Handler(void){
	//GPIOB ISR code:
	if (timer_notCounting){
		TIMER0->CTL |= 0x01; //Enable the timer  to start counting
		timer_notCounting = 0;
	}
	if ((GPIOB->MIS & 0x01) == 0x01){
		buttonPressed[0] = '1';
	}
	else if ((GPIOB->MIS & 0x02) == 0x02){
		buttonPressed[1] = '2';
	}
	else if ((GPIOB->MIS & 0x04) == 0x04){
		buttonPressed[2] = '3';
	}
	else if ((GPIOB->MIS & 0x08) == 0x08){
		buttonPressed[3] = 'x';
	}
	else if ((GPIOB->MIS & 0x10) == 0x10){
		buttonPressed[4] = '4';
	}
	else if ((GPIOB->MIS & 0x20) == 0x20){
		buttonPressed[5] = '5';
	}
	else if ((GPIOB->MIS & 0x40) == 0x40){
		buttonPressed[6] = '6';
	}
	GPIOB->ICR |= 0xFF; 
}


