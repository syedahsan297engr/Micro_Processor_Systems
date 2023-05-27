#include "stm32f4xx.h"
void configure_DC_MOTOR(void)
{
	int pin=0;
	RCC->AHB1ENR  |=0x4;//port C for motor
	//pin for motors
	for(pin=0; pin<=3; pin++)
	{
		GPIOC->MODER  &= ~(0x3<<(pin*2));
		GPIOC->MODER  |= (0x1<<(pin*2)); // output mode
		GPIOD->OSPEEDR |= (0x3<<(pin*2));
		GPIOC->OTYPER &= ~(0x1<<pin);
		GPIOC->PUPDR   &= ~(0x3<<(pin*2));
	}
}
void configure_STEPPER_MOTOR(void)
{
	int pin=0;
	RCC->AHB1ENR |= 0x02;
	//pin for motors PB0 PB1 PB2 PB3
	for(pin=0; pin<=3; pin++)
	{
		GPIOB->MODER  &= ~(0x3<<(pin*2));
		GPIOB->MODER  |= (0x1<<(pin*2)); // output mode
		GPIOB->OSPEEDR |= (0x3<<(pin*2));
		GPIOB->OTYPER &= ~(0x1<<pin);
		GPIOB->PUPDR   &= ~(0x3<<(pin*2));
	}
}
void configure_uart(void)
{
	int pin = 0;
	int baud_rate = 9600; // error is in the baud_rate
	float baud_div = 16000000 / (baud_rate * 16.0);
	int integer_part = 16000000 / (baud_rate * 16);
	int fractional_part = (baud_div - integer_part) * 16;
	
              // --- PC10 is UART4 TX and PC11 is UART4 RX
	RCC->AHB1ENR |= 0x01; // Enable Port A clock
	//RCC->AHB1ENR |= 0x08;
	// --- GPIO configuration
	pin = 0;
	//GPIOC ->AFR[1] &= ~(0xF << (pin-8)*4);
	//GPIOC ->AFR[1] |= (0x8 << (pin-8)*4);
	GPIOA ->AFR[0] &= ~(0xF << (pin)*4);
	GPIOA ->AFR[0] |= (0x8 << (pin)*4);
	GPIOA ->MODER &= ~(0x3 << pin*2);
	GPIOA ->MODER |= (0x2 << pin*2);
	GPIOA ->OTYPER &= ~(0x01 << pin); 
	GPIOA ->OSPEEDR |= (0x3 << pin*2);
	GPIOA ->PUPDR &= ~(0x3 << pin*2);
	GPIOA ->PUPDR |= (0x1 << pin*2);
	pin = 1;
	//GPIOC ->AFR[1] &= ~(0xF << (pin-8)*4);
	//GPIOC ->AFR[1] |= (0x8 << (pin-8)*4);
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
void configure_LCD(void)
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

void LCD_write_string(char *p){
	while(*p){
		LCD_write_data(*p);
		p++;
	}
}

void clear_DC_motor(void)
{
	    GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			GPIOC->ODR	&= ~(0x4);
			GPIOC->ODR	&= ~(0x8);
}
void clear_Stepper_motor(void)
{
	    GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
}
void SysTick_configure(void)
{
	SysTick->CTRL &= ~0x1;
	SysTick->LOAD = 4000-1; //for 250 us
	SysTick->VAL = 0;
	SysTick->CTRL = 0x7; //b2 = 1,b1 = 1,b0=1
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
void move_stepper_clock_wise(void){
	int i=0;
	int steps = 128;
	for( i = 0 ; i< steps ; i+=2){
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	|= (0x8);
			delay_ms(30);
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	|= (0x8);
			delay_ms(30);
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	|= (0x8);
			delay_ms(30);		
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);		
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);			
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);			
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);			
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
	}
}

void move_stepper_anticlock_wise(void){
	int i=0;
	int steps = 128;
	for( i = 0 ; i< steps ; i+=2){
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);	
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);		
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	|= (0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);			
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	&= ~(0x8);
			delay_ms(30);			
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	|= (0x4);
			GPIOB->ODR	|= (0x8);
			delay_ms(30);	
			GPIOB->ODR	&= ~(0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	|= (0x8);	
			delay_ms(30);			
			GPIOB->ODR	|= (0x1);
			GPIOB->ODR	&= ~(0x2);
			GPIOB->ODR	&= ~(0x4);
			GPIOB->ODR	|= (0x8);
	}
}
// main function
int main(void)
{
	int pin, test;
	configure_DC_MOTOR();
	configure_STEPPER_MOTOR();
	configure_uart();
	configure_LCD();
	while(1)
	{
		if(GPIOA->IDR&1)
		{
			//print_str("Hello World\n\r");
		}
		else
		{ }
		test = recv_char();
		
		if(test != -1) send_char(test);		

		if (test == '0'){
			//LCD_write_string("smart-mode");
			move_stepper_anticlock_wise();
			delay_ms(500);
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			delay_ms(10000); //rotate for 10s
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			delay_ms(500);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			delay_ms(10000);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			delay_ms(500);
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
			delay_ms(10000);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	&= ~(0x2);
			delay_ms(500);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
			delay_ms(10000);
			clear_DC_motor();
			move_stepper_clock_wise();
		} 
		if(test == '1' || test == '6'){
			//LCD_write_string("OFF");
			delay_ms(500);
			clear_DC_motor();
			clear_Stepper_motor();
		}
		if(test == '2'){
			//LCD_write_string("Clockwise mode");
			delay_ms(500);
			GPIOC->ODR	|= (0x1);
			GPIOC->ODR	&= ~(0x2);
		}
		if(test == '3'){
			//LCD_write_string("Anti-Clockwise mode");
			delay_ms(500);
			GPIOC->ODR	&= ~(0x1);
			GPIOC->ODR	|= (0x2);
		}
		if(test == '4'){
			//open drain
			//LCD_write_string("Drain Function");
			delay_ms(500);
			move_stepper_anticlock_wise();
		}
		if(test == '5'){
			//LCD_write_string("END");
			delay_ms(500);
			move_stepper_clock_wise();
			clear_Stepper_motor();
		}
	}
	return 0;
}