#include "stm32f4xx.h"
#include "cstdio"

void configure_ADC(void)
{
	//enable clock for ADC
	RCC->APB2ENR |= 0X100; //bit 8
	
	//ENBALE PORT A
	/*
	RCC->AHB1ENR |= 0X01;
	int pin = 0;
	GPIOA->MODER &= ~(0X1<<(pin*2));
	GPIOA->MODER |= (0X3<<(pin*2)); //configured as Analog
	GPIOA->OTYPER &= ~(0X1<<pin);
	GPIOA->OSPEEDR &= ~(0X1<<(pin*2));
	GPIOA->OSPEEDR |= (0X3<<(pin*2));
	GPIOA->PUPDR &= ~(0X1<<(pin*2));
	*/
	ADC1->CR1 = 0; 
	ADC1->CR2 = 0X401; // 0  1 10 11 30   ADON CONT EOCS ALIGN SWSTART        0100 0000 0011 operating in single conversion mode
	//Add Sampling Sequence
	ADC1->SQR3 |= 16<<0;
	ADC1->SQR3 |= 17<<5;
	//enable temperature sensor
	ADC->CCR |= 0XC00000; //temp sensor (Enable b22 and b23) //TSVREFEE VBATE pg 425
}

void configure_uart(void)
{
	int pin = 0;
	int baud_rate = 9600;
	float baud_div = 16000000 / (baud_rate * 16.0);
	int integer_part = 16000000 / (baud_rate * 16);
	int fractional_part = (baud_div - integer_part) * 16;
	
              // --- PC6 is USART6 TX and PC7 is USART6 RX
	RCC->AHB1ENR |= 0x04; // Enable Port C clock
	RCC->AHB1ENR |= 0x08;
	// --- GPIO configuration
	pin = 6;
	GPIOC ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOC ->AFR[0] |= (0x8 << (pin)*4);
	GPIOC ->MODER &= ~(0x3 << pin*2);
	GPIOC ->MODER |= (0x2 << pin*2);
	GPIOC ->OTYPER &= ~(0x01 << pin); 
	GPIOC ->OSPEEDR |= (0x3 << pin*2);
	GPIOC ->PUPDR &= ~(0x3 << pin*2);
	GPIOC ->PUPDR |= (0x1 << pin*2);
	pin = 7;
	GPIOC ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOC ->AFR[0] |= (0x8 << (pin)*4);
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
	RCC ->APB2ENR |= 0x20; // Enable USART6 Clock
	// UART Register configuration
	USART6->BRR = (integer_part << 4) | (fractional_part & 0xF);
	USART6->CR1 = 0x200C;
	USART6->CR2 = 0x0000;
	USART6->CR3 = 0x0;
}
// This function sends single ASCII character using USART6
void send_char(unsigned char c)
{
	while((USART6->SR & 0x80) == 0);// wait till TX ready
	USART6 ->DR = c;
}
// This function receives character using USART/UART6 in non-blocking method,
// Return -1 if nothing received yet
signed char recv_char(void)
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
	int pin,test;
	char str1[100];
	configure_uart();
	configure_ADC();
	print_str("UART and ADC Configuration\n\r");
	ADC1->CR2 |= 0x40000000; // enable bit 30 SWSTART Enabled now for starting conversion
	while(1)
	{
		if((ADC1->SR & 0x2) == 0x2){ //SR b1 EOC 
			int tmp = ADC1->DR; //Data stored at tmp
			
			sprintf(str1, "ADC Data = %i     \r", tmp);
			print_str(str1);
		}
		test = recv_char();
		
		if(test != -1) send_char(test);		

		if (test == '0'){
			print_str("LED OFF\n\r");
			pin = 14;
			GPIOD->ODR &= ~(0x1<<pin);
		}
		if (test == '1') 
		{
			print_str("LED ON\n\r");
			pin = 14;
			GPIOD->ODR |= (0x1<<pin);
		}
	}
}
