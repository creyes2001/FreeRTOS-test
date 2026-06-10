#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#define BUFFER_SIZE 20


typedef struct
{
	char data[BUFFER_SIZE];
	volatile uint8_t head;
	volatile uint8_t tail;
}buffer_t;

void Buffer_Init(buffer_t *buffer);
bool Buffer_Add(buffer_t *buffer,char element);
bool Buffer_Get(buffer_t *buffer,char *element);


#endif
