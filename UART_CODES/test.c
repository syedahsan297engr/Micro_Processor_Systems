/*
3.	Write a program that configures USART1 with baud rate 9600, 2 stop bit and no parity. 
The processor is operating at 32 MHz clock with APB2 peripheral operating at 8 MHz. 
The USART 1 is available at PA9 (TX) and PA10 (RX) when AF7 is enabled.
*/

#include "stm32f4xx.h"

void configure_uart(void)
{
	int pin = 0;
	int baud_rate = 9600;
	float baud_div = 32000000 / (baud_rate * 16.0);
	int integer_part = 32000000 / (baud_rate * 16);
	int fractional_part = (baud_div - integer_part) * 16;
	
              // --- PA9 is USART1 TX and PA10 is USART1 RX
	RCC->AHB1ENR |= 0x02; // Enable Port A clock
	RCC->AHB1ENR |= 0x08;
	// --- GPIO configuration
	pin = 6;
	GPIOA ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOA ->AFR[1] |= (0x8 << (pin)*4);
	/*
	alternate function select registers with 4 bits used to configure one pin
	see pic in word file
	*/
	GPIOA ->MODER &= ~(0x3 << pin*2);
	GPIOA ->MODER |= (0x2 << pin*2);
	GPIOA ->OTYPER &= ~(0x01 << pin); 
	GPIOA ->OSPEEDR |= (0x3 << pin*2);
	GPIOA ->PUPDR &= ~(0x3 << pin*2);
	GPIOA ->PUPDR |= (0x1 << pin*2);
	pin = 7;
	GPIOA ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOA ->AFR[0] |= (0x8 << (pin)*4);
	GPIOA ->MODER &= ~(0x3 << pin*2);
	GPIOA ->MODER |= (0x2 << pin*2);
	GPIOA ->OTYPER &= ~(0x01 << pin);
	GPIOA ->OSPEEDR |= (0x3 << pin*2);
	GPIOA ->PUPDR &= ~(0x3 << pin*2);
	GPIOA ->PUPDR |= (0x1 << pin*2);
	// --- USART Configuration
	// --- Enable USART1 clock 
	RCC ->APB2ENR |= 0x10; // Enable USART1 Clock
	// UART Register configuration
	USART1->BRR = (integer_part << 4) | (fractional_part & 0xF);
	USART1->CR1 = 0x200C; //enable uart b13 , b11 data bits , b10 parity, b9 parity select, b3 TE, b2 RE
	//                  0010 0000 0000 1100
	USART1->CR2 = 0x2000; //we have select 10 stop bit 2 at p13 and p12 FOR B12 AND B13
	USART1->CR3 = 0x0;
	
	pin = 13;
	GPIOD ->MODER &= ~(0x3 << pin*2);
	GPIOD ->MODER |= (0x1 << pin*2);
	GPIOD ->OTYPER &= ~(0x01 << pin);
	GPIOD ->OSPEEDR |= (0x3 << pin*2);
	GPIOD ->PUPDR &= ~(0x3 << pin*2);
}
// This function sends single ASCII character using USART6
void send_char(unsigned char c)
{
	while((USART1->SR & 0x80) == 0);// wait till TX ready
	USART1 ->DR = c;
}
// This function receives character using USART/UART6 in non-blocking method,
// Return -1 if nothing received yet
signed char recv_char(void)
{
	if ((USART1->SR & 0x20) == 0x20)
		return USART1 ->DR;
	else
		return -1;
}
// This function sends strings using USART/UART6
void print_str(char * str)
{
	while(*str != 0)
	{
		send_char(*str);
		str++;
	}
}
// main function
int main(void)
{
	int pin, test;
	configure_uart();
	
	while(1)
	{
		if(GPIOA->IDR&1)
		{
			print_str("Hello World\n\r");
		}
		else
		{ }
		test = recv_char();
		
		if(test != -1) send_char(test);		

		if (test == '0'){
			print_str("LED OFF\n\r");
			pin = 13;
			GPIOD->ODR &= ~(0x1<<pin);
		}
		if (test == '1') 
		{
			print_str("LED ON\n\r");
			pin = 13;
			GPIOD->ODR |= (0x1<<pin);
		}
	}
	return 0;
}
