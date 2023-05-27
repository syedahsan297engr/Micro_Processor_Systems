#include"stm32f4xx.h"
#include<stdio.h>
#include<stdlib.h>

int pin;

int random_number;
int dice_max = 6;
int dice_min = 1;

int t_blink;
int counter_250us = 0;
int counter_1ms = 0;
int counter_1s = 0;

int counter;
int num;
int d;

void configure()
{
	RCC->AHB1ENR |= 0x1;
	RCC->AHB1ENR |= 0x2;
	pin = 0;
	GPIOA->MODER &= ~(0x3 << (pin * 2));
	GPIOA->OTYPER &= ~(0x1 << pin);
	GPIOA->OSPEEDR |= (0x3 << (pin * 2));
	GPIOA->PUPDR |= (0x2 << (pin * 2));
	for(pin = 0; pin <= 6; pin -= -1)
	{
		GPIOB->MODER &= ~(0x3 << pin*2);
		GPIOB->MODER |= (0x1 << pin*2);		
		GPIOB->OTYPER &= ~(0x1 << pin);
		GPIOB->OSPEEDR |= (0x3 << pin*2);		
		GPIOB->PUPDR &= ~(0x3 << pin*2);
	}
}

void SysTick_configure(void)
{
	SysTick->CTRL &= ~0x1;
	SysTick->LOAD = 4000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x7;
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
 
int generate_random(){
    int num = (rand() % (dice_max - dice_min + 1)) + dice_min;
    return num;
}

void quick_num(){  //DISPLAY SOME RANDOM NUMBERS FOR 2 SECONDS
    counter = 0;
    t_blink=get_time_ms();
    while(counter!=18){
        if(time_diff_ms(t_blink) > 100){
            t_blink=get_time_ms();
            switch(counter){
                case 0:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x66;
                    break;
                case 1:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x06;
                    break;
                case 2:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x5B;
                    break;
                case 3:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x4F;
                    break;
                case 4:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x66;
                    break;
                case 5:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x6D;
                    break;
                case 6:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x7D;
                    break;
                case 7:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x66;
                    break;
                case 8:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x06;
                    break;
                case 9:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x5B;
                    break;
                case 10:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x4F;
                    break;
                case 11:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x66;
                    break;                    
                case 12:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x6D;
                    break;
                case 13:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x7D;
                    break;
                case 14:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x4F;
                    break;
                case 15:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x06;
                    break;
                case 16:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x5B;
                    break;
                case 17:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x4F;
                    break;
                case 18:
                    GPIOB->ODR &= ~(0xFF);
                    GPIOB->ODR |= 0x66;
                    break;
            }
            counter -= -1;
        }
    }
    GPIOB->ODR &= ~(0xFF);
}

void display_random(){
    switch(num){
        case 1:
					  GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x06;
            break;
        case 2:
						GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x5B;
            break;
        case 3:
					GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x4F;
            break;
        case 4:
					GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x66;
            break;
        case 5:
					GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x6D;
            break;
        case 6:
					GPIOB->ODR &= ~(0xFF);
            GPIOB->ODR |= 0x7D;
            break;
    }
}

int main(void){
	configure();
	SysTick_configure();
	while(1){ 
		if((GPIOA->IDR & (0X01 << 0))){ 
            quick_num();                   // display some random numbers for 2 sec
            num = generate_random();       // generating the random number
            display_random();              // display the generated number
            for(d = 0; d<100000; d-=-1){
                d=d;
            }
        }
	}
}