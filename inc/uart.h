#ifndef UART_H
#define UART_H

#include "stm32f446xx.h"
#include <stdint.h>

void Uart_Init(void);
void Uart_Tx(uint8_t data);
uint8_t Uart_Rx(void);

#endif //UART_H
