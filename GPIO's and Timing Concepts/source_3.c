#include "stm32f4xx.h"

void configure(void)
{
	int pin = 0;
	//E D C B A
	RCC->AHB1ENR |= 0x10; //port E enabled
	//PE1 and PE3 configured
	for(pin = 1 ; pin<=3 ; pin+=2)
	{
		GPIOE->MODER &= ~(0x3 << pin * 2); 
		GPIOE->OTYPER &= ~(0x1 << pin); // no push_pull
		GPIOE->OSPEEDR &= ~(0x3 << pin * 2);
		GPIOE->OSPEEDR |= (0x3 << pin * 2); //high speed
		GPIOE->PUPDR &= ~(0x3 << pin * 2); 
	}

	RCC->AHB1ENR = 0x02;
	pin = 2;
	GPIOB->MODER &= ~(0x3 << pin * 2);
	GPIOB->MODER |= (0x1 << pin * 2);
	GPIOB->OTYPER &= ~(0x1 << pin);
	GPIOB->OSPEEDR &= ~(0x3 << pin * 2);  
	GPIOB->OSPEEDR |= (0x3 << pin * 2);
	GPIOB->PUPDR &= ~(0x3 << pin * 2);
	GPIOB->PUPDR |= (0x0 << pin * 2);
}

int main(void)
{
	int sw1_p = 1;
	int sw3_p = 3;
	int pin;
	configure();
	while (1)
	{
		//check if SW1 and SW3 are pressed or not
		if((GPIOE->IDR & (0x1<<sw1_p)) && (GPIOE->IDR & (0x1<<sw3_p)))
		{
			pin = 2;
			GPIOB->ODR |= (0x1<<pin);
		}
		else {
			pin = 2;
			GPIOB->ODR &= ~(0x1<<pin);
		}
	}
}
