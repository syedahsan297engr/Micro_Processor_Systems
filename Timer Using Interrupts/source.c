#include "stm32f4xx.h"
int counter_250us = 0;
int counter_1ms = 0;
int counter_1s = 0;


void configure(void)
{
	int pin=0;
	
	RCC->AHB1ENR  |=0x2; //Enabling port B
	RCC->AHB1ENR  |=0x10; //Enabling port E
	
	//Configuring from PB0-PB7
	for(pin=0;pin<=7;pin++)
	{
		GPIOB->MODER  &=~(0x3<<(pin*2));
		GPIOB->MODER  |=(0x1<<(pin*2));
		GPIOB->OTYPER &=~(0x1<<pin);
		GPIOB->OSPEEDR |=(0x3<<(pin*2));
		GPIOB->PUPDR   &=~(0x3<<(pin*2));
	}
	//Configuring from PE0-PE2
	for(pin=0;pin<=2;pin++)
	{
		GPIOE->MODER  &=~(0x3<<(pin*2));
		GPIOE->MODER  |=(0x1<<(pin*2));
		GPIOE->OTYPER &=~(0x1<<pin);
		GPIOE->OSPEEDR |=(0x3<<(pin*2));
		GPIOE->PUPDR   &=~(0x3<<(pin*2));
	}
}

void SysTick_configure(void)
{
	SysTick->CTRL &= ~0x1;
	SysTick->LOAD = 4000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x7; //b2 = 1,b1 = 1,b0=1
}

void SysTick_Handler(void)
{
	counter_250us++;
	if(counter_250us%4 == 0)
	{
		counter_1ms++;
	}
	if(counter_1ms%1000 == 0)
	{
		counter_1s++;
	}
}

int get_time_ms(void)
{
	return counter_1ms;
}
int time_diff_ms(int to)
{
	return counter_1ms-to;
}

void delay_us()
{
	int i=0;
	for(i=0;i<2	;i++)
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
void LCD_write_command(unsigned char cmd)
{
	GPIOE->ODR	&=~(0x1); //RS=0
	GPIOE->ODR	&=~(0x2); //RW=0
	GPIOE->ODR	|=(0x4); //E = 1
	GPIOB->ODR	&=~(0xFF); //Set all zeros 
	GPIOB->ODR	|=cmd; //Place the cmd
	delay(2);
	GPIOE->ODR	&=~(0x4); //E = 0
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
	delay(2);
	GPIOE->ODR	&=~(0x4);
	delay(2000);
}

void LCD_write_string(char *p){
	while(*p){
		LCD_write_data(*p);
		p++;
	}
}

void LCD_write_message(void)
{
	LCD_write_command(0xC0);
	LCD_write_string("     Counter");
}

void delay_ms(int ms)
{
		int i,j;
		for(i=0;i<ms;i++)
		{
			for(j=0;j<4;j++);
			while((SysTick->CTRL & 0x10000)  == 0);
		}
}


int main(void)
{
	int ctr=1;
	int ctr1=0;
	int t_blink=0;
	configure();
	LCD_configure();
	SysTick_configure();
	t_blink=get_time_ms();
	
  LCD_write_command(0x80+0x8);
	LCD_write_data(':');
  LCD_write_command(0x80+0x7);
	LCD_write_data('0');
  LCD_write_command(0x80+0x6);
	LCD_write_data('0');

	while(1)
	{
		if(time_diff_ms(t_blink)>1000)
		{
			//LED BLINK
			t_blink = get_time_ms();
			//LCD_write_command(0x02);
			LCD_write_command(0x80+0x9);
			LCD_write_data('0'+(((ctr1/10))%10));
		  LCD_write_command(0x80+0xA);
			LCD_write_data('0'+(ctr1++%10));
	
			if(ctr1%60 == 0)
			{			
				LCD_write_command(0x80+0x6);
				LCD_write_data('0'+(((ctr/10))%10));
				LCD_write_command(0x80+0x7);
				LCD_write_data('0'+(ctr++%10));
				ctr1 = 0;
			}
			
		}
	}
}