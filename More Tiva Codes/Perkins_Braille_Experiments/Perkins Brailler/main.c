#include "TM4C123.h"                    // Device header
#include "INIT.h"
#include "UART0.h"
#include <string.h>
#define BS 0x08	//Backspace
#define LF 0x0A //Line Feed
#define CR 0x0D //Carriage Return

char buttonPressed[10] = "000000000";
volatile uint16_t timer_notCounting = 1;
volatile uint16_t check_buttonPressed = 0;
volatile uint16_t caps_lock = 0;
volatile uint16_t slideSwitch = 0;
void clearArray(char* buttonArray);

int main(){

	Init(); //Initialization function

	while(1){
		/*The main code checks the combination of button presses
		before the timer stopped counting.
		*/
		slideSwitch = GPIOE->DATA & 0x02;
		if (check_buttonPressed){
			
			if (!strcmp(buttonPressed,"100000000")){
				
				if (slideSwitch) UART_OutChar('1');
				else{
					if (!caps_lock) UART_OutChar('a');
				  else UART_OutChar('A');
				}	
			}
			
			else if (!strcmp(buttonPressed,"120000000")){
				
				if (slideSwitch) UART_OutChar('2');
				else{
					if (!caps_lock) UART_OutChar('b');
					else UART_OutChar('B');
				}
			}
			
			else if (!strcmp(buttonPressed,"100040000")){
				if (slideSwitch) UART_OutChar('3');
				else{
					if (!caps_lock) UART_OutChar('c');
					else UART_OutChar('C');
				}
			}
			
			else if (!strcmp(buttonPressed,"100045000")){
				if (slideSwitch) UART_OutChar('4');
				else{
					if (!caps_lock) UART_OutChar('d');
					else UART_OutChar('D');
				}
			}
			
			else if (!strcmp(buttonPressed,"100005000")){
				if (slideSwitch) UART_OutChar('5');
				else{
					if (!caps_lock) UART_OutChar('e');
					else UART_OutChar('E');
				}
			}
			
			else if (!strcmp(buttonPressed,"120040000")){
				if (slideSwitch) UART_OutChar('6');
				else{
					if (!caps_lock) UART_OutChar('f');
					else UART_OutChar('F');
				}
			}
			
			else if (!strcmp(buttonPressed,"120045000")){
				if (slideSwitch) UART_OutChar('7');
				else{
					if (!caps_lock) UART_OutChar('g');
					else UART_OutChar('G');
				}
			}
			
			else if (!strcmp(buttonPressed,"120005000")){
				if (slideSwitch) UART_OutChar('8');
				else{
					if (!caps_lock) UART_OutChar('h');
					else UART_OutChar('H');
				}
			}
			
			else if (!strcmp(buttonPressed,"020040000")){
				if (slideSwitch) UART_OutChar('9');
				else{
					if (!caps_lock) UART_OutChar('i');
					else UART_OutChar('I');
				}
			}
			
			else if (!strcmp(buttonPressed,"020045000")){
				if (slideSwitch) UART_OutChar('0');
				else{
					if (!caps_lock) UART_OutChar('j');
					else UART_OutChar('J');
				}
			}
			
			else if (!strcmp(buttonPressed,"103000000")){
				if (!caps_lock) UART_OutChar('k');
				else UART_OutChar('K');
			}
			
			else if (!strcmp(buttonPressed,"123000000")){
				if (!caps_lock) UART_OutChar('l');
				else UART_OutChar('L');
			}
			
			else if (!strcmp(buttonPressed,"103040000")){
				if (!caps_lock) UART_OutChar('m');
				else UART_OutChar('M');
			}
			
			else if (!strcmp(buttonPressed,"103045000")){
				if (!caps_lock) UART_OutChar('n');
				else UART_OutChar('N');
			}
			
			else if (!strcmp(buttonPressed,"103005000")){
				if (!caps_lock) UART_OutChar('o');
				else UART_OutChar('O');
			}
			
			else if (!strcmp(buttonPressed,"123040000")){
				if (!caps_lock) UART_OutChar('p');
				else UART_OutChar('P');
			}
			
			else if (!strcmp(buttonPressed,"123045000")){
				if (!caps_lock) UART_OutChar('q');
				else UART_OutChar('Q');
			}
			
			else if (!strcmp(buttonPressed,"123005000")){
				if (!caps_lock) UART_OutChar('r');
				else UART_OutChar('R');
			}
			
			else if (!strcmp(buttonPressed,"023040000")){
				if (!caps_lock) UART_OutChar('s');
				else UART_OutChar('S');
			}
			
			else if (!strcmp(buttonPressed,"023045000")){
				if (!caps_lock) UART_OutChar('t');
				else UART_OutChar('T');
			}
			
			else if (!strcmp(buttonPressed,"103000600")){
				if (!caps_lock) UART_OutChar('u');
				else UART_OutChar('U');
			}
			
			else if (!strcmp(buttonPressed,"123000600")){
				if (!caps_lock) UART_OutChar('v');
				else UART_OutChar('V');;
			}
			
			else if (!strcmp(buttonPressed,"020045600")){
				if (!caps_lock) UART_OutChar('w');
				else UART_OutChar('W');
			}
			
			else if (!strcmp(buttonPressed,"103040600")){
				if (!caps_lock) UART_OutChar('x');
				else UART_OutChar('X');
			}
			
			else if (!strcmp(buttonPressed,"103045600")){
				if (!caps_lock) UART_OutChar('y');
				else UART_OutChar('Y');
			}
			
			else if (!strcmp(buttonPressed,"103005600")){
				if (!caps_lock) UART_OutChar('z');
				else UART_OutChar('Z');
			}
			
			else if (!strcmp(buttonPressed,"000x00000")){
				UART_OutChar(' ');
			}
			
			else if (!strcmp(buttonPressed,"000000070")){
				UART_OutChar(BS);
				UART_OutChar(' ');
				UART_OutChar(BS);
			}
			
			else if (!strcmp(buttonPressed,"000000008")){
				UART_OutChar(CR);
				UART_OutChar(LF);
			}
			
			clearArray(buttonPressed);
			check_buttonPressed = 0;
			
		}
	}
}

void clearArray(char* buttonArray){
	for (int i = 0; i < 9; i++){
			buttonArray[i] = '0';
	}
}

void TIMER0A_Handler(){
	/*This ISR executes when the timer stops counting*/
	timer_notCounting = 1;
	check_buttonPressed = 1;
	TIMER0->ICR |= 0x01;
}

void GPIOB_Handler(){
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
	else if ((GPIOB->MIS & 0x80) == 0x80){
		buttonPressed[7] = '7';
	}
	GPIOB->ICR |= 0xFF; 
}

void GPIOE_Handler(){
	//GPIOE ISR code:
	if (timer_notCounting){
		TIMER0->CTL |= 0x01; //Enable the timer  to start counting
		timer_notCounting = 0;
	}

	if ((GPIOE->MIS & 0x04) == 0x04){
		caps_lock ^= 1;
	}
	if ((GPIOE->MIS & 0x08) == 0x08){
		buttonPressed[8] = '8';
	}
	GPIOE->ICR |= 0xFF;
}
