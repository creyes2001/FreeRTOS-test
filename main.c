#include "stm32f446xx.h"
#include "FreeRTOSConfig.h"
#include "gpio.h"

void SystemInit(void) {}

gpio_config_t led= {
	.mode = GPIO_PIN,
	.direction = GPIO_OUTPUT,
	.resistor = DISABLE,
	.outType = PUSH_PULL,
	.speed = MEDIUM_SPEED
};

gpio_config_t push_button = {
	.mode = GPIO_PIN,
	.direction = GPIO_INPUT,
	.resistor = PULL_DOWN
};

int main(void)
{
	RCC->AHB1ENR |= (1 << 0);   // GPIOAEN
	GPIO_Init(5,PORTA,&led);
	GPIO_Init(6,PORTA,&push_button);

    while (1)
	{
		if(GPIO_READ(6,PORTA) == GPIO_HIGH){
			GPIO_Write(5,PORTA,GPIO_HIGH);
		}
		GPIO_Write(5,PORTA,GPIO_LOW);
	}
}
