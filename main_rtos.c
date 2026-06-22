#include "stm32f446xx.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "clock.h"

void SystemInit(void) {}

volatile uint32_t ms_ticks = 0;

void UART4_IRQHandler(void)
{
	Uart_InterruptHandler();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask; (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for(;;);
}

gpio_config_t led= {
	.mode = GPIO_PIN,
	.direction = GPIO_OUTPUT,
	.resistor = RESISTOR_DISABLE,
	.outType = PUSH_PULL,
	.speed = MEDIUM_SPEED
};

gpio_config_t led2= {
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

TaskHandle_t led1_handle, led2_handle;

void parse_and_dispatch(const char *cmd){
	char led = cmd[0];
	uint32_t rate = atoi(&cmd[1]);
	if(rate == 0) return;

	if(led == 'A')
	{
		xTaskNotify(led1_handle,rate,eSetValueWithOverwrite);
	}
	
	if(led == 'B')
	{
		xTaskNotify(led2_handle,rate,eSetValueWithOverwrite);
	}

}

void uart_task(void *p){
	char buff[8];
	uint8_t idx = 0;
	char c;
	for(;;){
		if(Uart_Rx(&c))
		{
			if(c == '\n' || c =='\r')
			{
				parse_and_dispatch(&buff);
				idx = 0;
			}
			else
			{
				buff[idx] = c;
				idx++;
			}
		}
		else
		{
       		vTaskDelay(pdMS_TO_TICKS(5));
		}
	}
}

void blink_task(void *p) {
    uint32_t rate = 1000;
	uint32_t new_rate;
	for(;;) {
		GPIO_Write(2,PORTA,GPIO_HIGH);
        if(xTaskNotifyWait(0,0,&new_rate,pdMS_TO_TICKS(rate) == pdTRUE))
		{
			rate = new_rate;
		}
				
		GPIO_Write(2,PORTA,GPIO_LOW);
        if(xTaskNotifyWait(0,0,&new_rate,pdMS_TO_TICKS(rate) == pdTRUE))
		{
			rate = new_rate;
		}
    }
}

void blink_task1(void *p) {
    for(;;) {
		GPIO_Write(5,PORTA,GPIO_HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
		GPIO_Write(5,PORTA,GPIO_LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


int main(void)
{
	SysClk_Config();
	
	RCC->AHB1ENR |= (1 << 0);   // GPIOAEN
	(void)RCC->AHB1ENR;			// dummy read
								
	RCC->APB1ENR |= (1 << 19);	//Enable UART4 clock
	(void)RCC->APB1ENR;			// dummy read

	GPIO_Init(0,PORTA,&uart4_tx);
	GPIO_Init(1,PORTA,&uart4_rx);
	GPIO_Init(2,PORTA,&led2);
	GPIO_Init(5,PORTA,&led);
	
	Uart_Init();
 	
	xTaskCreate(blink_task, "led1_task", 128, NULL, 1, led1_handle);
	xTaskCreate(blink_task1, "led2_task", 128, NULL, 1, led2_handle);
	xTaskCreate(uart_task, "uarttx", 256, NULL, 1, NULL);
    vTaskStartScheduler();
    for(;;);  // should never reach here

}
