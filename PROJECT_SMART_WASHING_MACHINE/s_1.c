#include"stm32f4xx.h"
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
	for(pin=0; pin<=3; pin++)
	{
		GPIOC->MODER  &= ~(0x3<<(pin*2));
		GPIOC->MODER  |= (0x1<<(pin*2)); // output mode
		GPIOD->OSPEEDR |= (0x3<<(pin*2));
		GPIOC->OTYPER &= ~(0x1<<pin);
		GPIOC->PUPDR   &= ~(0x3<<(pin*2));
	}
	
	pin =15;
	GPIOD->MODER &= ~(0x3<<(pin*2));//output 
  GPIOD->MODER |= (0x1<<(pin*2)); // output set 01
  GPIOD->OTYPER &= ~(0x1<<(pin));//push/pull
	GPIOD->OSPEEDR |= (0x3<<(pin*2));// max speed
	GPIOD->PUPDR &= ~(0x3<<(pin*2));//no pull up/down
}
void clear_motor(void)
{
	    GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
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

int main(void)
{
	configure();
	clear_motor();
	GPIOD->IDR |= (0x1<<0);
	while(1)
	{
			if(GPIOA->IDR & 0x1){
				delay(20000);
				GPIOD->ODR |= (0x1<<15);
				GPIOC->ODR	|= (0x1);
				GPIOC->ODR	&= ~(0x2);
			}
			/*
			if(GPIOD->IDR & 0x1){
				GPIOC->ODR	&= ~(0x1);
				GPIOC->ODR	|= (0x2);
			}
			*/
			else{
				delay(20000);
				GPIOC->ODR	&= ~(0x1);
				GPIOC->ODR	|= (0x2);
				//GPIOD->ODR |= (0x1<<15);
			}
			//will rate 1 motor in forward direction
		
	}
}

#include "stm32f4xx.h"
void configure(void)
{
	int pin=0;
	
	RCC->AHB1ENR  |=0x2;
	RCC->AHB1ENR  |=0x10;
	
	for(pin=0;pin<=7;pin++)
	{
		GPIOB->MODER  &=~(0x3<<(pin*2));
		GPIOB->MODER  |=(0x1<<(pin*2));
		GPIOB->OTYPER &=~(0x1<<pin);
		GPIOB->OSPEEDR |=(0x3<<(pin*2));
		GPIOB->PUPDR   &=~(0x3<<(pin*2));
	}
	for(pin=0;pin<=2;pin++)
	{
		GPIOE->MODER  &=~(0x3<<(pin*2));
		GPIOE->MODER  |=(0x1<<(pin*2));
		GPIOE->OTYPER &=~(0x1<<pin);
		GPIOE->OSPEEDR |=(0x3<<(pin*2));
		GPIOE->PUPDR   &=~(0x3<<(pin*2));
	}
}
void delay_us()
{
	int i=0;
	for(i=0;i<2000;i++)
	{
		;//NOP
	}
}
void delay(int us)
{
	int i=0;
	for(i=0;i<us;i++)
	{
		delay_us();
	}
}
void LCD_write_command(unsigned char cmd)
{
	GPIOE->ODR	&=~(0x1);
	GPIOE->ODR	&=~(0x2);
	GPIOE->ODR	|=(0x4);
	GPIOB->ODR	&=~(0xFF);
	GPIOB->ODR	|=cmd;
	delay(10);
	GPIOE->ODR	&=~(0x4);
	delay(2000);

}
void LCD_configure(void)
{
	LCD_write_command(0x38);
	LCD_write_command(0x06);
	LCD_write_command(0x02);
	LCD_write_command(0x01);
	LCD_write_command(0x0F);
}

void LCD_write_data(unsigned char data)
{
	GPIOE->ODR	|=(0x1);
	GPIOE->ODR	&=~(0x2);
	GPIOE->ODR	|=(0x4);
	GPIOB->ODR	&=~(0xFF);
	GPIOB->ODR	|=data;
	delay(10);
	GPIOE->ODR	&=~(0x4);
	delay(2000);
}

void LCD_write_message(void)
{
	LCD_write_data('H');
	LCD_write_data('e');
	LCD_write_data('l');
	LCD_write_data('l');
	LCD_write_data('o');

}
int main(void)
{
	configure();
	LCD_configure();
	
	LCD_write_message();
	while(1)
	{
			//Blink LED
	}
	return 0;
}