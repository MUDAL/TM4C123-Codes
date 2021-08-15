#include "stm32f4xx.h"                  // Device header

/*
Simple program to configure the Systick timer to count periodically
*/

void Clocks_Init(void);
void GPIOA_Init(void);
void SysTick_Init(void);

int main(void)
{
	Clocks_Init();
	GPIOA_Init();
	SysTick_Init();
	
	while(1)
	{
	}
	
}

void Clocks_Init(void)
{
	while (!(RCC->CR & RCC_CR_HSIRDY)){}//wait for HSI clock to stabilize
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN); //clock enable for GPIOA
}

void GPIOA_Init(void)
{
	GPIOA->MODER |= GPIO_MODER_MODE5_0; //PA5 as output
}

void SysTick_Init(void)
{
	//Systick default clock source = AHB clock / 8 = 2MHz
	SysTick->LOAD = 2000000 - 1; //load value equivalent to 1Hz timer frequency
	SysTick->VAL = 0;
	SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk); //Enable systick interrupt and systick timer
}

extern "C" void SysTick_Handler(void)
{
	GPIOA->ODR ^= GPIO_ODR_OD5; //toggle LED(PA5) at 1Hz frequency
}

