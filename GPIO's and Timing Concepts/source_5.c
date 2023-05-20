#include "stm32f4xx.h"

void configure(void)
{
	int pin = 0;
	//E D C B A
	RCC->AHB1ENR |= 0x04; //port C enabled
	//PC1 and PC3 configured
	for(pin = 1 ; pin<=3 ; pin+=2)
	{
		GPIOC->MODER &= ~(0x3 << pin * 2);
		GPIOC->OTYPER &= ~(0x1 << pin); // no push_pull
		GPIOC->OSPEEDR &= ~(0x3 << pin * 2);
		GPIOC->OSPEEDR |= (0x3 << pin * 2); //high speed
		GPIOC->PUPDR &= ~(0x3 << pin * 2);
	}
	RCC->AHB1ENR = 0x02; //port B enbled
	//PB3 and PB5 configured
	for(pin = 3 ; pin<=5 ; pin+=2)
	{
		GPIOB->MODER &= ~(0x3 << pin * 2);
		GPIOB->MODER |= (0x1 << pin * 2);
		GPIOB->OTYPER &= ~(0x1 << pin);
		GPIOB->OSPEEDR &= ~(0x3 << pin * 2);  
		GPIOB->OSPEEDR |= (0x3 << pin * 2);
		GPIOB->PUPDR &= ~(0x3 << pin * 2);
	}
}

/*
program that turns On D1 when SW1 is pressed. (D1 connected to PB3) (SW1 connected to PC1)
The program should turn off D2 when SW2 is pressed. SW2 remains ON when SW2 is depressed.
*/
int main(void)
{
	int sw1_p = 1;
	int sw2_p = 3;
	int pin;
	configure();
	while (1)
	{
		if(GPIOC->IDR & (0x1<<sw1_p))
		{
			pin = 3;
			GPIOB->ODR |= (0x1<<pin);
		}
		if(GPIOC->IDR & (0x1<<sw2_p))
		{
			pin = 5;
			GPIOB->ODR &= ~(0x1<<pin);
		}
		else
		{
			pin = 3;
			GPIOB->ODR &= ~(0x1<<pin);
			pin = 5;
			GPIOB->ODR |= (0x1<<pin);
		}
	}
}
/*
	int pin;
	pin = 1;
	configure();
	while (1)
	{
		pin = 1;
		if(GPIOC->IDR & (0x1<<1))
		{
			pin = 3;
			GPIOB->ODR |= (0x1<<3);
		}
		pin = 3;
		if(GPIOC->IDR & (0x1<<3))
		{
			pin = 5;
			GPIOB->ODR &= ~(0x1<<5);
		}
		else
		{
			pin = 3;
			GPIOB->ODR &= ~(0x1<<3);
			pin = 5;
			GPIOB->ODR |= (0x1<<5);
		}
	}
*/
