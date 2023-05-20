#include "stm32f4xx.h"

void configure(void)
{
	int pin = 0;
	// Enable clock for port C (RCC register bit 2 set to 1)
	RCC->AHB1ENR |= 0x4;
	// Update MODE register for port C pin 12 to input
	pin = 12;
	GPIOC->MODER &= ~(0x3 << pin * 2);
	GPIOC->MODER |= (0x0 << pin * 2); // This statement can be skipped due to 0
	// Update pin output type register to push-pull
	GPIOC->OTYPER &= ~(0x1 << pin);
	// Update pin speed register to high speed
	GPIOC->OSPEEDR &= ~(0x3 << pin * 2);
	GPIOC->OSPEEDR |= (0x3 << pin * 2);
	// Update pin pull down register to pull down
	GPIOC->PUPDR &= ~(0x3 << pin * 2);
	GPIOC->PUPDR |= (0x2 << pin * 2);

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
}

int main(void)
{
	configure();
	while (1)
	{
		// Check if SW1 is pressed
		if (GPIOC -> IDR & 0x1000) // bit12 b1 0000 0000 0000 = 0x1000
		{
			// Turn on D1
			GPIOC->ODR ^= (0x10); // set bit7
		}
	}
}
