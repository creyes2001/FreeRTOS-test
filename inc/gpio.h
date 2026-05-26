#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum {
	GPIO_PIN,
	AF_PIN,
	ANALOG_PIN
}gpio_mode_e;

typedef enum {
	GPIO_OUTPUT,
	GPIO_INPUT
}gpio_direction_e;

typedef enum {
	PULL_UP,
	PULL_DOWN,
	RESISTOR_DISABLE
}gpio_resistor_e;

typedef enum {
	PUSH_PULL,
	OPEN_DRAIN
}gpio_outType_e;

typedef enum {
	HIGH_SPEED,
	FAST_SPEED,
	MEDIUM_SPEED,
	LOW_SPEED
}gpio_speed_e;

typedef enum {
	GPIO_HIGH,
	GPIO_LOW
}gpio_level_e;

//Only for the stm32f446 in LQFP64 MCU 
typedef enum {
	PORTA,
	PORTB,
	PORTC,
	PORTD,
	PORTH
}gpio_port_e;

typedef enum {
	AF0 = 0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15,
}alt_func_e;


//

typedef struct {
	gpio_mode_e  mode;
	gpio_direction_e direction;
	gpio_resistor_e resistor;
	gpio_outType_e outType;
	gpio_speed_e speed;

	//for stm32f446 in LQFP64 MCU 
	alt_func_e alt_func;

}gpio_config_t;

void GPIO_Init(uint8_t pin, gpio_port_e port, gpio_config_t *config);
void GPIO_Write(uint8_t pin, gpio_port_e port,gpio_level_e level);
gpio_level_e GPIO_READ(uint8_t pin, gpio_port_e port);

#endif //GPIO_H
