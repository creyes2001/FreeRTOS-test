#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum {
	GPIO_PIN,
	AF_PIN,
	ANALOG_PIN
}gpio_type_e;

typedef enum {
	GPIO_OUTPUT,
	GPIO_INPUT
}gpio_direction_e;

typedef enum {
	PULL_UP,
	PULL_DOWN,
	DISABLE
}gpio_resistor_e;

typedef enum {
	PUSH_PULL,
	OPEN_DRAIN
}gpio_outType_e;

typedef enum {
	HIGH_SPEED,
	MEDIUM_SPEED,
	LOW_SPEED
}gpio_speed_e;

typedef enum {
	GPIO_HIGH,
	GPIO_LOW
}gpio_level_e;

typedef struct {
	gpio_type_e  type;
	gpio_direction_e direction;
	gpio_resistor_e resistor;
	gpio_outType_e outType;
	gpio_speed_e speed;
}gpio_config_t;

void GPIO_Init(uint8_t PIN, gpio_config_t *config);
void GPIO_Write(uint8_t PIN,gpio_level_e level);
gpio_level_e GPIO_READ(uint8_t PIN);

#endif //GPIO_H
