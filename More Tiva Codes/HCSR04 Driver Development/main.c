#include "TM4C123.h"                    // Device header

//DATE:: 
//Driver development for HCSR-04 ultrasonic sensor

//STEPS
//1.)Register configurations
//2.)Declarations and definitions

//Trigger timers -> timer 0 and timer 1
//Echo timers -> timer 2 and timer 3

/*Trigger sequence

* Timer 0 starts (HIGH is sent to trigger pin of sensor)
* After 10uS Timer 0 stops and Timer 1 starts (LOW is sent)
* After 40uS Timer 1 stops and Timer 0 starts (HIGH is sent again)
* REPEAT

*/

/*Echo sequence (Input Capture)

NB: Timers 2 and 3 are configured the same way
*When rising edge of echo is detected, capture the input with Timer 2 (START)
*When falling edge of echo is detected, capture the input with Timer 3 (STOP)

PULSE-WIDTH ALGORITHM:
if STOP > START: width = STOP - START
else:						 width = LOAD + (STOP - START)

*/

/* IMPORTANT TIMER CONFIGURATION INFORMATION
 TIMER | INTERRUPT NO | SET ENABLE REGISTER | BIT 
	0A	 |     19       |     EN0							|  19
	1A 	 |     21       |     EN0             |  21
  2A	 |     23       |     EN0             |  23
	3A	 |     35       |     EN1             |  03
			 
*/
int main(){
	
}

