#include "stm32f4xx.h"
int counter_250us = 0;
int counter_1ms = 0;
int counter_1s = 0;

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