#ifndef UART_H
#define UART_H

#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>

void Uart_Init(void);
void Uart_Tx(uint8_t data);
bool Uart_Rx(uint8_t *data);
void Uart_InterruptHandler(void);

#endif //UART_H
