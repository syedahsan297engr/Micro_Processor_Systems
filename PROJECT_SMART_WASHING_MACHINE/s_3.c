#include "stm32f4xx.h"

void configure(void)
{
	int pin=0;
	RCC->AHB1ENR  |=0x4;//port C for motor
	RCC->AHB1ENR |= 0x01;
	RCC->AHB1ENR |= 0x08;
	
	pin=0;
	GPIOA->MODER &= ~(0x3<<(pin*2));//input 00
  GPIOA->OTYPER &= ~(0x1<<(pin)); //push/pull
	GPIOA->OSPEEDR |= (0x3<<(pin*2));// max speed 11
	GPIOA->PUPDR &= ~(0x3<<(pin*2));//no pull up/down 00
	
	pin=0;
	GPIOD->MODER &= ~(0x3<<(pin*2));//input 00
	GPIOD->MODER |= (0x1<<(pin*2));//input 00
  GPIOD->OTYPER &= ~(0x1<<(pin)); //push/pull
	GPIOD->OSPEEDR |= (0x3<<(pin*2));// max speed 11
	GPIOD->PUPDR &= ~(0x3<<(pin*2));//no pull up/down 00
	
	//pin for motors
	for(pin=0; pin<=3; pin++)
	{
		GPIOC->MODER  &= ~(0x3<<(pin*2));
		GPIOC->MODER  |= (0x1<<(pin*2)); // output mode
		GPIOD->OSPEEDR |= (0x3<<(pin*2));
		GPIOC->OTYPER &= ~(0x1<<pin);
		GPIOC->PUPDR   &= ~(0x3<<(pin*2));
	}
	
	//test pin
	pin =15;
	GPIOD->MODER &= ~(0x3<<(pin*2));//output 
  GPIOD->MODER |= (0x1<<(pin*2)); // output set 01
  GPIOD->OTYPER &= ~(0x1<<(pin));//push/pull
	GPIOD->OSPEEDR |= (0x3<<(pin*2));// max speed
	GPIOD->PUPDR &= ~(0x3<<(pin*2));//no pull up/down
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
	// --- GPIO configuration
	pin = 6;
	GPIOC ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOC ->AFR[0] |= (0x8 << (pin)*4);
	/*
	alternate function select registers with 4 bits used to configure one pin
	see pic in word file
	*/
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
	// --- USART Configuration
	// --- Enable USART1 clock 
	RCC ->APB2ENR |= 0x20; // Enable USART6 Clock
	// UART Register configuration
	USART6->BRR = (integer_part << 4) | (fractional_part & 0xF);
	USART6->CR1 = 0x200C; //enable uart b13 , b11 data bits , b10 parity, b9 parity select, b3 TE, b2 RE
	//                  0010 0000 0000 1100
	USART6->CR2 = 0x0000; //we have select 00 stop bit 1 at p13 and p12
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

void delay_us()
{
	int i=0;
	for(i=0;i<2000;i++)
	{
		;//NOP
	}
	//Have used <2000 to control delays keep in mind the frequency of your processor
}
void delay(int us)
{
	int i=0;
	for(i=0;i<us;i++)
	{
		delay_us();
	}
}

void clear_motor(void)
{
	    GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
}

void move_stepper_clock_wise(void){
	int i=0;
	int steps = 256;
	for( i = 0 ; i< steps ; i++){
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	|= (0x8);
			delay(100);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	|= (0x8);
			delay(100);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	|= (0x8);
			delay(100);		
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);		
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);			
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);			
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);			
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
	}
}

void move_stepper_anticlock_wise(void){
	int i=0;
	int steps = 128;
	for( i = 0 ; i< steps ; i++){
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);		
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);		
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);			
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	&= ~(0x8);
			delay(100);			
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	|= (0x4);
			GPIOC->ODR	|= (0x8);
			delay(100);	
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	|= (0x8);	
			delay(100);			
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	|= (0x8);
	}
}

// main function
int main(void)
{
	int test;
	configure();
	configure_uart();
	clear_motor();
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
			move_stepper_anticlock_wise();
		} 
		if(test == '4'){
			clear_motor();
		}
	}
	return 0;
}
