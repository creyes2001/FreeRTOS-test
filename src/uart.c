#include "uart.h"

void Uart_Init(void){
	
	UART4->CR1 = 0;
    UART4->CR2 = 0;
    UART4->CR3 = 0;
	
	UART4->BRR = (24 << 4) | 7;   // = 0x187, for 115200 @ 45 MHz
	
	UART4->CR1 |= (1U << 3); //Transmitter enable (TE)
	UART4->CR1 |= (1U << 2); //Receiver enable (RE)
	UART4->CR1 |= (1U << 13); //Uart peripherial enable (UE)

	
}

void Uart_Tx(uint8_t data){
	while (!(UART4->SR & (1U << 7)));
	UART4->DR = data;
}

uint8_t Uart_Rx(void){
	if(UART4->SR & (1U << 5))
	{
		UART4->SR &= ~(1U <<5);
		return UART4->DR;
	}
	return 123;
}
