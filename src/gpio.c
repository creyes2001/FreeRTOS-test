#include "gpio.h"
#include "stm32f446xx.h"

//Using the GPIO_Typedef declaration from the CMSIS header 
GPIO_TypeDef * const gpio_port[] = {GPIOA,GPIOB,GPIOC,GPIOD,GPIOH};

void GPIO_Init(uint8_t pin, gpio_port_e port, gpio_config_t *config)
{

	gpio_port[port]->MODER &= ~(3 << (2^pin));//Clear bits before configuration

	switch(config->mode){
		case GPIO_PIN:
			if(config->direction == GPIO_OUTPUT)
			{
				gpio_port[port]->MODER |= (1 << (2*pin));
			}
			else
			{
				gpio_port[port]->MODER &= ~(3 << (2*pin));
			}
			break;
		case AF_PIN:
			gpio_port[port]->MODER |= (1 << (2*pin));
			break;
		case ANALOG_PIN:
			gpio_port[port]->MODER |= (3 << (2*pin));
			break;
		default:
			break;
	}
	
	if(config->outType == PUSH_PULL)
		{
			gpio_port[port]->OTYPER &= ~(1 << pin); 
		}
	else
		{
			gpio_port[port]->OTYPER |= (1 << pin); 
		}


	switch(config->resistor){
		case PULL_DOWN:
			gpio_port[port]->PUPDR |= (2 << (2*pin)); 
			break;
		case PULL_UP:
			gpio_port[port]->PUPDR |= (1 << (2*pin));
			break;
		case DISABLE:
			gpio_port[port]->PUPDR &= ~(3 << (2*pin));
			break;
		default:
			break;
	}
	
	switch(config->speed){
		case HIGH_SPEED:
			gpio_port[port]->OSPEEDR |= (3 << (2*pin));
			break;
		case FAST_SPEED:
			gpio_port[port]->OSPEEDR |= (2 << (2*pin));
			break;
		case MEDIUM_SPEED:
			gpio_port[port]->OSPEEDR |= (1 << (2*pin));
			break;
		case LOW_SPEED:
			gpio_port[port]->OSPEEDR &= ~(3 << (2*pin));
			break;
		default:
			break;
	}	
}

void GPIO_Write(uint8_t pin, gpio_port_e port,gpio_level_e level)
{
	if(level == GPIO_HIGH)
	{
		gpio_port[port]->BSRR = (1 << pin);
	}
	else
	{
		gpio_port[port]->BSRR = (1 << (pin + 16));
	}

}

gpio_level_e GPIO_READ(uint8_t pin, gpio_port_e port)
{
	if(	gpio_port[port]->IDR &= (1 << pin))
	{
		return GPIO_HIGH;
	}
	return GPIO_LOW;
}


