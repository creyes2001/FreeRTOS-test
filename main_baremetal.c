#include "stm32f446xx.h"
#include "gpio.h"
#include "uart.h"
#include "clock.h"

void SystemInit(void) {}

volatile uint32_t ms_ticks = 0;

void UART4_IRQHandler(void)
{
	Uart_InterruptHandler();
}


gpio_config_t led= {
	.mode = GPIO_PIN,
	.direction = GPIO_OUTPUT,
	.resistor = RESISTOR_DISABLE,
	.outType = PUSH_PULL,
	.speed = MEDIUM_SPEED
};

gpio_config_t push_button = {
	.mode = GPIO_PIN,
	.direction = GPIO_INPUT,
	.resistor = PULL_DOWN
};

//UART5 pin configuration

gpio_config_t uart4_tx = {
	.mode = AF_PIN,
	.outType = PUSH_PULL,
	.speed = HIGH_SPEED,
	.resistor = RESISTOR_DISABLE,
	.alt_func = AF8
};

gpio_config_t uart4_rx = {
	.mode = AF_PIN,
	.speed = HIGH_SPEED,
	.alt_func = AF8
};


int main(void)
{
	SysClk_Config();
	
	RCC->AHB1ENR |= (1 << 0);   // GPIOAEN
	(void)RCC->AHB1ENR;			// dummy read
								
	RCC->APB1ENR |= (1 << 19);	//Enable UART4 clock
	(void)RCC->APB1ENR;			// dummy read

	GPIO_Init(5,PORTA,&led);
	GPIO_Init(0,PORTA,&uart4_tx);
	GPIO_Init(1,PORTA,&uart4_rx);

	Uart_Init();
 	
	while(1)
	{

		Uart_Tx('A');
    	uint8_t rx;
    	if (Uart_Rx(&rx)) {
        	// success path — toggle LED if rx == 'X'
        	if (rx == 'X') GPIO_Write(5, PORTA, GPIO_HIGH);
    	}
	}
}
