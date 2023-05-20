#include "stm32f4xx.h"

void configure(void)
{
	int pin = 0;
	// Enable clock for port C (RCC register bit 2 set to 1)
	RCC->AHB1ENR |= 0x4;
	RCC->AHB1ENR |= 0x8;
	// Update MODE register for port C pin 12 to input
	pin = 0;
	GPIOA->MODER &= ~(0X3<<(pin*2));
	GPIOA->OTYPER &= ~(0X1<<(pin));
	GPIOA->OSPEEDR |= (0X3<<(pin*2));
	GPIOA->PUPDR &= ~(0X3<<(pin*2));
	pin = 1;
	GPIOA->MODER &= ~(0X3<<(pin*2));
	GPIOA->MODER |= (0X1<<(pin*2));
	GPIOA->OTYPER &= ~(0X1<<(pin));
	GPIOA->OSPEEDR |= (0X3<<(pin*2));
	GPIOA->PUPDR &= ~(0X3<<(pin*2));

	// Update MODE register for port C pin 7 to output
	pin = 7;
	GPIOC->MODER &= ~(0x3 << pin * 2);
	GPIOC->MODER |= (0x1 << pin * 2);
	// Update pin output type register to push-pull
	GPIOC->OTYPER &= ~(0x1 << pin);
	// Update pin speed register to high speed
	GPIOC->OSPEEDR &= ~(0x3 << pin * 2);  // This statment can be skipped due to 3
	GPIOC->OSPEEDR |= (0x3 << pin * 2);
	// Update pin pull down register to no-pull
	GPIOC->PUPDR &= ~(0x3 << pin * 2);
	GPIOC->PUPDR |= (0x0 << pin * 2); // This statment can be skipped due to 0
	
	pin = 13;
	GPIOD->MODER &= ~(0x3 << pin * 2);
	GPIOD->MODER |= (0x1 << pin * 2);
	GPIOD->OTYPER &= ~(0x1 << pin);
	GPIOD->OSPEEDR &= ~(0x3 << pin * 2);  // This statment can be skipped due to 3
	GPIOD->OSPEEDR |= (0x3 << pin * 2);
	GPIOD->PUPDR &= ~(0x3 << pin * 2);
}

int main(void)
{
	int pin;
	configure();
	while(1)
	{
		if(GPIOA->IDR & 0X1)
		{
			pin = 13;
			GPIOD->ODR |= (0X1<<pin);
			
		}
		else
		{
			pin = 1;
			GPIOA->ODR |= (0X1<<pin);
		}
	}
}
