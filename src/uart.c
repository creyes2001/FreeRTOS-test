#include "buffer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

static buffer_t tx_buffer;
static buffer_t rx_buffer;

void Uart_Init(void){
	
	UART4->CR1 = 0;
    UART4->CR2 = 0;
    UART4->CR3 = 0;
	
	UART4->BRR = (24 << 4) | 7;   // = 0x187, for 115200 @ 45 MHz
	
	UART4->CR1 |= (1U << 3); //Transmitter enable (TE)
	UART4->CR1 |= (1U << 2); //Receiver enable (RE)
	UART4->CR1 |= (1U << 13); //Uart peripherial enable (UE)

	NVIC_EnableIRQ(UART4_IRQn);
	UART4->CR1 |= (1U << 7); //TXEIE enabled
	UART4->CR1 |= (1U << 5); //RXEIE enabled

	Buffer_Init(&tx_buffer);
	Buffer_Init(&rx_buffer);
}

void Uart_InterruptHandler(void){
	
	UBaseType_t status;

	//tx handling
	if ((UART4->SR & (1U << 7)) && (UART4->CR1 & (1U <<7))) { 
		uint8_t c;
		status = taskENTER_CRITICAL_FROM_ISR();
		bool has = Buffer_Get(&tx_buffer,&c); 
		taskEXIT_CRITICAL_FROM_ISR(status);
		if(has)
		{
			UART4->DR = c;
		}
		else {
			UART4->CR1 &= ~(1U << 7);
		}
	}

	//rx handling
	if(UART4->SR & (1U << 5)){
		uint8_t c = UART4->DR;
		status = taskENTER_CRITICAL_FROM_ISR();
		(void)Buffer_Add(&rx_buffer,c);
		taskEXIT_CRITICAL_FROM_ISR(status);
	}
}

void Uart_Tx(uint8_t data){
	bool ok;
	taskENTER_CRITICAL();
	ok = Buffer_Add(&tx_buffer,data);
	taskEXIT_CRITICAL();
	(void)ok;
 	UART4->CR1 |= (1U << 7); //TXEIE enabled
}

bool Uart_Rx(uint8_t *data){
	bool ok;
	taskENTER_CRITICAL();
	ok = Buffer_Get(&rx_buffer,data);
	taskEXIT_CRITICAL();
	return ok;
}
