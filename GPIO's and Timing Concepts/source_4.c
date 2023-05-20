#include "stm32f4xx.h"
void configure(void)
{
	int pin=0;
	
	RCC->AHB1ENR  |=0x01; //Enabling port A
	RCC->AHB1ENR  |=0x08; //Enabling port D
	
	//Configuring from PB0-PB7
	for(pin=0;pin<=7;pin++)
	{
		GPIOA->MODER  &=~(0x3<<(pin*2));
		GPIOA->MODER  |=(0x1<<(pin*2));
		GPIOA->OTYPER &=~(0x1<<pin);
		GPIOA->OSPEEDR |=(0x3<<(pin*2));
		GPIOA->PUPDR   &=~(0x3<<(pin*2));
	}
	//Configuring PD1 PD3 PD5 using for loop with increment of 2
	for(pin=1;pin<=5;pin+=2)
	{
		GPIOD->MODER  &=~(0x3<<(pin*2));
		GPIOD->MODER  |=(0x1<<(pin*2));
		GPIOD->OTYPER &=~(0x1<<pin);
		GPIOD->OSPEEDR |=(0x3<<(pin*2));
		GPIOD->PUPDR   &=~(0x3<<(pin*2));
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
void LCD_write_command(unsigned char cmd)
{
	GPIOD->ODR	&=~(0x1); //RS=0
	GPIOD->ODR	&=~(0x2); //RW=0
	GPIOD->ODR	|=(0x4); //E = 1
	GPIOA->ODR	&=~(0xFF); //Set all zeros 
	GPIOA->ODR	|=cmd; //Place the cmd
	delay(10);
	GPIOD->ODR	&=~(0x4); //E = 0
	delay(2000);

}
void LCD_configure(void)
{
	//configure LCD for 2 rows, small fonts with 8 pin interface
	LCD_write_command(0x38);
	//0 0 0 0 1 DL N F x x
	//DL = High 8-bit bus mode with mpu DL = Low 4-bit Bus mode with mpu
	//N = Low 1 line mode, N = High 2 line mode
	//F = Low 5x8 dot format display, F = High 5x11 dot format display
	//0 0 0 0 1 1 1 0 0 0 
	//0x38
	LCD_write_command(0x06); //entry mode set and increment in cursor (100) in this mode we set the moving direction of cursor and display
	LCD_write_command(0x02); //return home instruction
	LCD_write_command(0x01); //clear display command refer to manual
	LCD_write_command(0x0F); //Display ON/OFF
	//1DCB = 1111 = F 
	//D = 1 whole display turned on, C = 1 cursor turned on, B = 1 cursor Blink on
}

void LCD_write_data(unsigned char data)
{
	GPIOD->ODR	|=(0x1);
	GPIOD->ODR	&=~(0x2);
	GPIOD->ODR	|=(0x4);
	GPIOA->ODR	&=~(0xFF);
	GPIOA->ODR	|=data;
	delay(10);
	GPIOD->ODR	&=~(0x4);
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
	LCD_write_string("Hello World");
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