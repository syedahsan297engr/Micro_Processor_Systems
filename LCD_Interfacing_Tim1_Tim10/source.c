#include "stm32f4xx.h"
#include "misc.h"

int counter_250us = 0;
int counter_1ms = 0;
int counter_1s = 0;

void configure(void)
{
	int pin = 0;
	
	RCC->AHB1ENR |= 0x2; // Enalbe port B clock
	RCC->AHB1ENR |= 0x10; // Enalbe port E clock
	RCC->AHB1ENR |= 0x08; // Enalbe port D clock
	
	// Port B
	for(pin = 0; pin <= 7; pin ++)
	{
		GPIOB->MODER &= ~(0x3 << (pin * 2));
		GPIOB->MODER |= (0x1 << (pin * 2));
		
		GPIOB->OTYPER &= ~(0x1 << pin);
		
		GPIOB->OSPEEDR |= (0x3 << (pin * 2));
		
		GPIOB->PUPDR &= ~(0x3 << (pin * 2));
	}
	
	// Port E
	for(pin = 0; pin <= 2; pin ++)
	{
		GPIOE->MODER &= ~(0x3 << (pin * 2));
		GPIOE->MODER |= (0x1 << (pin * 2));
		
		GPIOE->OTYPER &= ~(0x1 << pin);
		
		GPIOE->OSPEEDR |= (0x3 << (pin * 2));
		
		GPIOE->PUPDR &= ~(0x3 << (pin * 2));
	}
	
	// Port E
	for(pin = 12; pin <= 15; pin ++)
	{
		GPIOD->MODER &= ~(0x3 << (pin * 2));
		GPIOD->MODER |= (0x1 << (pin * 2));
		
		GPIOD->OTYPER &= ~(0x1 << pin);
		
		GPIOD->OSPEEDR |= (0x3 << (pin * 2));
		
		GPIOD->PUPDR &= ~(0x3 << (pin * 2));
	}
}

void delay_us(void)
{
	int i = 0;
	for(i=0;i<2;i++)
	;
}

void delay(int us)
{
	int i = 0;
	for (i = 0; i < us ; i++)
	{
		delay_us();
	}
}

void LCD_write_command(unsigned char cmd)
{
	GPIOE->ODR &= ~(0x1); // RS = 0
	GPIOE->ODR &= ~(0x2); // RW = 0
	GPIOE->ODR |= (0x4); // E = 1
	GPIOB->ODR &= ~(0xFF);
	GPIOB->ODR |= cmd;
	delay(2);
	GPIOE->ODR &= ~(0x4); // E = 0
	delay(100);
}

void LCD_write_data(unsigned char data)
{
	GPIOE->ODR |= (0x1); // RS = 1
	GPIOE->ODR &= ~(0x2); // RW = 0
	GPIOE->ODR |= (0x4); // E = 1
	GPIOB->ODR &= ~(0xFF);
	GPIOB->ODR |= data;
	delay(2);
	GPIOE->ODR &= ~(0x4); // E = 0
	delay(100);
}

void LCD_configure(void)
{
	LCD_write_command(0x38); // 8 data lines, 2 rows, small font
	LCD_write_command(0x06); // Increment  cursor
	LCD_write_command(0x02); // Return home
	LCD_write_command(0x01); // Clear display
	LCD_write_command(0x0F); // Display ON, cursor ON, blink ON
}

int get_time_ms(void)
{
	return counter_1ms;
}

int time_diff_ms(int t0)
{
	return counter_1ms - t0;
}

void delay_ms(int msec)
{
	int i, j;
	for(i = 0; i< msec ; i++)
	{
		for(j=0; j< 4; j++)
		{
			while((SysTick->CTRL&0x1000) == 0);
		}
	}
}

void TIM1_configure(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// Enalbe Timer 1 Clock
	RCC->APB2ENR |= 0x1;
	
	
	// Enalbe timer 1 for 1 milli second
	TIM1->CR1  &= ~0x1;
	// Xd = (16 MHz)(1ms) = 16 kHz
	TIM1->ARR   = 8000-1;
	TIM1->PSC   = 2-1;
	TIM1->CNT   = 0;
	TIM1->SR   &= ~0x1; // UIF  = 0
	TIM1->DIER = 0x1; // UIE = 1
	//TIM1->RCR = 0;
	TIM1->CR1  |= 0x11;
	__enable_irq();
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int msec_ctr = 0; // global variable

int sec_ctr = 0; 	// global variable for seconds
int mint_ctr = 1; // global variable for minutes

int t0 = 0;
int main(void)
{
	configure();
	TIM1_configure();
	
	
	t0 = msec_ctr; // assign the value
	// write in the LCD
	LCD_write_command(0x80 + 0x8);
	LCD_write_data(':');
	LCD_write_command(0x80 + 0x7);
	LCD_write_data('0');
	LCD_write_command(0x80 + 0x6);
	LCD_write_data('0');
	while(1)
	{
		if (msec_ctr - t0 > 1000) // 1 sec has been passed
		{
			// make changings in the LCD
			LCD_write_command(0x80+0x9);
			LCD_write_data('0'+(((sec_ctr/10))%10));
			LCD_write_command(0x80+0xA);
			LCD_write_data('0'+(sec_ctr%10));
			
			sec_ctr = sec_ctr + 1;	// seconds will be incremented at every if condition
			// ----------------------------
			if (sec_ctr %60 == 0)
			{
				LCD_write_command(0x80+0x6);
				LCD_write_data('0'+(((mint_ctr/10))%10));
				LCD_write_command(0x80+0x7);
				LCD_write_data('0'+(mint_ctr%10));
				
				mint_ctr = mint_ctr + 1;
				sec_ctr = 0; // assign sec to zero.
			}
			t0 = msec_ctr; // assign again
		}
	}
	
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if (TIM1->SR & 0x1)
	{
		msec_ctr = msec_ctr + 1; // update mili second counter
		TIM1->SR &= ~0x1; // Interrupt acknowledgement
	}
}