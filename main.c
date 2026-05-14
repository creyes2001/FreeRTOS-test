#include "stm32f446xx.h"
#include "gpio.h"

void SystemInit(void) {}

gpio_config_t led= {
	.mode = GPIO_PIN,
	.direction = GPIO_OUTPUT,
	.resistor = DISABLE,
	.outType = PUSH_PULL,
	.speed = MEDIUM_SPEED
};

int main(void)
{
	RCC->AHB1ENR |= (1 << 0);   // GPIOAEN
//	GPIOA->MODER &= ~(3 << 10); // Clear bits 11:10
//	GPIOA->MODER |=  (1 << 10); // Set to 01 (output)
		GPIO_Init(5,PORTA,&led);
    while (1)
	{
		GPIO_Write(5,PORTA,GPIO_HIGH);
		for (volatile int i = 0; i < 500000; i++); // Crude delay
		GPIO_Write(5,PORTA,GPIO_LOW);
		for (volatile int i = 0; i < 500000; i++); // Crude delay
	
		}
}
