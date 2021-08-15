#include <stdint.h>

uint8_t counter1 = 0;

/*void add(void){
	//static uint8_t operand = 0;
	uint8_t operand = 0;
	while(1){
		operand++;
		if (operand == 10){
			return;
		}	
	}
}*/

int main(){
	
	//uint8_t counter1 = 0;
	//add();
	uint8_t *p_Counter;
	p_Counter = &counter1;
	while(*p_Counter < 21){
		++(*p_Counter);
	}
	
	/*while(counter1 < 21){
		++counter1;
	}*/
	
	
}

