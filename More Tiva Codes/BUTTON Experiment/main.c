/*
* THE NUMBERED COMMENTS SHOULD BE TRANSFERRED AS TESTING CODE
* TO THE PERKINS BRAILLER CODE TO HELP BATTLE "DELAYED PRESS EFFECT"
*
* THE DELAYED PRESS EFFECT OCCURS WHEN ONE BUTTON IS PRESSED AND ANOTHER 
* IS PRESSED AFTER A LONG TIME MAKES SOMETHING HAPPEN WHICH 
*	ISN'T WHAT WE WANT.
*
*/

#include "TM4C123.h"                    // Device header
#include <stdlib.h>
#include <string.h>
volatile unsigned long read1;
volatile unsigned long read2;

unsigned long timeDiff(volatile unsigned long t1, volatile unsigned long t2);

int start; //1
int end;   //2
char buttonPressed[6] = "00000";
int scan(char* detect_button); //3
int delayedPress(void); //4
void deadLoop(void); //5
int colour = 0; 
unsigned long time = 0;

void Timer_Init(void);

int main(){
	
	
	SYSCTL->RCGCGPIO |= 0x20; //clock for port F
	SYSCTL->RCGCGPIO |= 0x10; //Enable clock for port E
	//Disable PLL and using 16MHz clock source (MOSC)
	SYSCTL->RCC |= 0x800; //BYPASS PLL
	SYSCTL->RCC &= ~0x400000; //Disable system clock divisor
	
	//======= unlock PF0 =======
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR = 0x01;
	//==========================
	GPIOF->DIR &= ~0x11; //make PF4 and PF0 input pins
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x1F;
	GPIOF->PUR |= 0x11; //enable pullup for PF4 and PF0
	
	__disable_irq();
	
	Timer_Init(); //Initialize timer0A
	GPIOF->IS &= ~0x11; //make PF4 and PF0 edge sensitive
	GPIOF->IBE &= ~0x11; //not enabling interrupts for both edges
	GPIOF->IEV &= ~0x11; //falling edge detection
	GPIOF->ICR |= 0x11; //clear any prior interrupt
	GPIOF->IM |= 0x11; //unmask interrupt
	
	NVIC->IP[30] = 1<<5; //set priority 1 (for GPIOF)
	NVIC->ISER[0] = 0x40000000; //enable IRQ30
	
	__enable_irq();
	
	while(1){
		
//		switch (colour){
//			case 1:
//				GPIOF->DATA |= 0x04;
//				break;
//			
//			case 2:
//				GPIOF->DATA = 0x0E;
//				break;
//				
//			case 3:
//				GPIOF->DATA = 0x04;
//				break;
//		}
//		
//		if (!strcmp(buttonPressed,"12000")){
//			colour = 1;
//			strcpy(buttonPressed,"00000");
//		}
		time = timeDiff(read1,read2);
		if (read1 != 0 && read2 != 0){
			if (time < 200000){
				GPIOF->DATA |= 0x02;
			}
			read1 = 0;
			read2 = 0;
		}
	}
}

void TIMER0A_Handler(){
	/*This ISR is executed if both buttons are not 
	pressed seemingly at the same time*/
//	if (buttonPressed[0] == '1') buttonPressed[0] = '0';
//	if (buttonPressed[1] == '2') buttonPressed[1] = '0';
	if (read1 != 0) read1 = 0;
	else if (read2 != 0) read2 = 0;
	TIMER0->ICR |= 0x01;
}

void GPIOF_Handler(void){
	TIMER0->CTL |= 0x01; //Enable the timer  to start counting
	if ((GPIOF->MIS &0x10) == 0x10) {//buttonPressed[0] = '1';
		read1 = TIMER0->TAR;
	}
	else if ((GPIOF->MIS &0x01) == 0x01) {//buttonPressed[1] = '2';
		read2 = TIMER0->TAR;
	}
	GPIOF->ICR |= 0x11; 
}

void Timer_Init(){
	SYSCTL->RCGCTIMER |= 0x01; // Timer 0 clock
	TIMER0->TAMR |= 0x1; //One shot Timer Mode
  TIMER0->TAMR |= 0x10; //Enable 32 bit timer to count down
  TIMER0->TAILR = 16000000 - 1; 
	TIMER0->IMR |= 0x01; //Enable interrupts for the timer
	NVIC->ISER[0] = 0x80000; //enable IRQ19
	NVIC->IP[19] = 2<<5; //set priority 2 for IRQ19
}

int scan(char* detect_button){
    int count = 0;
    for (int i = 0; i < 5; i++){
        if (detect_button[i] != '0'){
            count++;
        }
    }
    if (count > 1){
        return 1;
    }
    else{
        return 0;
    }
}

int delayedPress(){
	start = scan(buttonPressed);
	deadLoop();
	end = scan(buttonPressed);
	if (start == end) return 1;
	else return 0;
}

void deadLoop(){
	for (int i = 0; i < 20000; i++){}
}

unsigned long timeDiff(volatile unsigned long t1, volatile unsigned long t2){
	unsigned long retval;
	if (t1 > t2){
		retval = t1 - t2;
	}
	else{
		retval = t2 - t1;
	}
	return retval;
}


