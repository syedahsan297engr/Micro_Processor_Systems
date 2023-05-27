/*
1.Write a program that configures UART4 with baud rate 115200, 1 stop bit and odd parity. 
The processor is operating at 16 MHz clock with APB1 peripheral operating at 4 MHz. 
The UART 4 is available at PC10 (TX) and PC11 (RX) when AF8 is enabled.
*/

#include "stm32f4xx.h"

void configure_uart(void)
{
	int pin = 0;
	int baud_rate = 115200;
	float baud_div = 16000000 / (baud_rate * 16.0);
	int integer_part = 16000000 / (baud_rate * 16);
	int fractional_part = (baud_div - integer_part) * 16;
	
              // --- PC10 is UART4 TX and PC11 is UART4 RX
	RCC->AHB1ENR |= 0x04; // Enable Port C clock
	RCC->AHB1ENR |= 0x08;
	// --- GPIO configuration
	pin = 10;
	GPIOC ->AFR[1] &= ~(0xF << (pin-8)*4);
	GPIOC ->AFR[1] |= (0x8 << (pin-8)*4);
	GPIOC ->MODER &= ~(0x3 << pin*2);
	GPIOC ->MODER |= (0x2 << pin*2);
	GPIOC ->OTYPER &= ~(0x01 << pin); 
	GPIOC ->OSPEEDR |= (0x3 << pin*2);
	GPIOC ->PUPDR &= ~(0x3 << pin*2);
	GPIOC ->PUPDR |= (0x1 << pin*2);
	pin = 11;
	GPIOC ->AFR[1] &= ~(0xF << (pin-8)*4);
	GPIOC ->AFR[1] |= (0x8 << (pin-8)*4);
	GPIOC ->MODER &= ~(0x3 << pin*2);
	GPIOC ->MODER |= (0x2 << pin*2);
	GPIOC ->OTYPER &= ~(0x01 << pin);
	GPIOC ->OSPEEDR |= (0x3 << pin*2);
	GPIOC ->PUPDR &= ~(0x3 << pin*2);
	GPIOC ->PUPDR |= (0x1 << pin*2);
	
	pin = 14;
	GPIOD ->MODER &= ~(0x3 << pin*2);
	GPIOD ->MODER |= (0x1 << pin*2);
	GPIOD ->OTYPER &= ~(0x01 << pin);
	GPIOD ->OSPEEDR |= (0x3 << pin*2);
	GPIOD ->PUPDR &= ~(0x3 << pin*2);
	// --- USART Configuration
	// --- Enable USART1 clock 
	RCC ->APB1ENR |= 0x80000; // Enable UART4 Clock for this b19 should be set 1
	// UART Register configuration
	UART4->BRR = (integer_part << 4) | (fractional_part & 0xF);
	//have to select odd parity so b9 is 1
	UART4->CR1 = 0x220C; //enable uart b13 , b11 data bits , b10 parity, b9 parity select, b3 TE, b2 RE
	//                  0010 0110 0000 1100
	UART4->CR2 = 0x0000; //we have select 00 stop bit 1 at p13 and p12
	UART4->CR3 = 0x0;
}
// This function sends single ASCII character using USART6
void send_char(unsigned char c)
{
	while((UART4->SR & 0x80) == 0);// wait till TX ready
	UART4 ->DR = c;
}
// This function receives character using USART/UART6 in non-blocking method,
// Return -1 if nothing received yet
signed char recv_char(void)
{
	if ((UART4->SR & 0x20) == 0x20)
		return UART4 ->DR;
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
		print_str("2020-EE-297\n\r");
	}
	return 0;
}