#include "stm32f446xx.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "clock.h"

void SystemInit(void) {}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask; (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for(;;);
}

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

void blink_task(void *p) {
    for(;;) {
        GPIOA->ODR ^= (1 << 5);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
	SysClk_Config();
	RCC->AHB1ENR |= (1 << 0);   // GPIOAEN
	GPIO_Init(5,PORTA,&led);

 	xTaskCreate(blink_task, "blink", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    for(;;);  // should never reach here

}
