#include "uart.h"

void Uart_Init(void){
	
	UART4->CR1 &= ~USART_CR1_UE;
	UART4->CR1 = 0;
    UART4->CR2 = 0;
    UART4->CR3 = 0;
//	UART4->BRR = (4687 << 4) | (8 << 0);  // 9600 baud @ 45 MHz
//	UART4->BRR = (390U << 4) | (10U << 0);
//UART4->BRR = (2343 << 4) | (12 << 0);	
UART4->BRR = (24 << 4) | 7;   // = 0x187, for 115200 @ 45 MHz
	UART4->CR1 |= (1U << 3); //Transmitter enable (TE)
	UART4->CR1 |= (1U << 13); //Uart peripherial enable (UE)

	
/*// Enable USART2 clock (APB1ENR bit 17, not 19)
RCC->APB1ENR |= (1U << 17);

// PA2 = USART2_TX, alternate function AF7 (NOT AF8)
GPIOA->MODER  &= ~(3U << (2 * 2));
GPIOA->MODER  |=  (2U << (2 * 2));
GPIOA->AFR[0] &= ~(0xFU << (2 * 4));
GPIOA->AFR[0] |=  (7U   << (2 * 4));   // AF7 for USART2

// USART2 base = 0x40004400, same BRR math (same 45 MHz)
USART2->CR1 = 0;
USART2->CR2 = 0;
USART2->CR3 = 0;
USART2->BRR = (390U << 4) | (10U << 0);
USART2->CR1 |= (1U << 3);    // TE
USART2->CR1 |= (1U << 13);   // UE*/
}

void Uart_Tx(uint8_t data){
	while (!(UART4->SR & (1U << 7)));
	UART4->DR = data;

/*	while (!(USART2->SR & (1U << 7)));  // wait for TXE (data register empty)
    USART2->DR = data;*/
}
