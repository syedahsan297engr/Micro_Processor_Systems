#include "stm32f4xx.h"

// Objective:		Send 0 or 1 from PC to Microcontroller and when MicroController accept this particular data, it should print a message which show number in English.

void configure_usart(void)
{
	// initialize all the variables and assign values to them
	int pin, baud_rate, integer_part, floating_part;
	float division;
	
	baud_rate = 9600;
	division = (16000000) / ((16.0)*(baud_rate));
	integer_part = (16000000) / ((16)*(baud_rate));
	floating_part = (division-integer_part)*16;     // 2*2*2*2 = 16 as 4 bits are for storing floating point numbers
	
	// step 1: Enable the clock USART6
	RCC->APB2ENR = RCC->APB2ENR | 0x20;			// bit 5 is intersting.
	// step 2: Assign value of Baud Rate Register for USART6
	USART6->BRR = (integer_part<<4)|(floating_part & 0xF);
	// step 3: Assign values to CR1 and CR2
	USART6->CR1 |= (0x4|0x8|0x2000);  // enable the transmitter(b2), enable the receiver(b3), enable the transmitter(b2), Enable UART (b13)
	USART6->CR2 |= (0x2000);  // choose the stop bit (b12 and b13) 00: 1 bit, 10: 2 bits
  // step 4: Configure the GPIOs
  RCC->AHB1ENR |= 0x4; 						// Enable port C
	RCC->AHB1ENR |= 0x08;
	// --- GPIO configuration
	for(pin = 6; pin<=7; pin ++)
	{
		GPIOC->MODER &=  ~(0x3 << (pin*2)); 		// make it alternate
		GPIOC->MODER |=  (0x2 << (pin*2)); 		// make it alternate
		
		GPIOC->AFR[0] &= ~(0xf << (pin * 4)); // clear all  // error in this line
		GPIOC->AFR[0] |= (0x8 << (pin * 4)); // enter 8
	
		GPIOC->OTYPER &= ~(0x1 << (pin*1));		// Push Pull
		GPIOC->OSPEEDR |= (0x3 << (pin*2));		// very high speed 
		
		GPIOC->PUPDR &= ~(0x3 << (pin*2));
		//GPIOC->PUPDR |=  (0x1 << (pin*2));    // make it pull up No Problem

	}
	pin = 14;
	GPIOD ->MODER &= ~(0x3 << pin*2);
	GPIOD ->MODER |= (0x1 << pin*2);
	GPIOD ->OTYPER &= ~(0x01 << pin);
	GPIOD ->OSPEEDR |= (0x3 << pin*2);
	GPIOD ->PUPDR &= ~(0x3 << pin*2);
	return;
}
// This function sends single ASCII character using USART6

void send_char(unsigned char c) // Transmitter is Empty  | b7 
{
	while((USART6->SR & 0x80) == 0);// wait till TX ready
	USART6 ->DR = c;
}
// This function receives character using USART/UART6 in non-blocking method,
// Return -1 if nothing received yet
signed char recv_char(void)            // Receiver is Not Empty | b5
{
	if ((USART6->SR & 0x20) == 0x20)
		return USART6 ->DR;
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

int main(void)
{
	int test,pin;
	configure_usart();
	while(1)
	{
		test = recv_char();
		
		if(test != -1) 
		{
			print_str("Number ");
			send_char(test);
			print_str (" is entered by PC\n\n\r");
		}
				

		if (test == '0') 
		{
			print_str("Zero is send by the Controller\n\n\r");
			pin = 14;
			GPIOD->ODR &= ~(0x1<<pin);
		}
		if (test == '1'){
			print_str("One is send by the Controller\n\n\r");
			pin = 14;
			GPIOD->ODR |= (0x1<<pin);
		}		
		
	}
	
	return 0;
}