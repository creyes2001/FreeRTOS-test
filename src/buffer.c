#include "buffer.h"

void Buffer_Init(buffer_t *buffer)
{
	buffer->head = 0;
	buffer->tail = 0;
}

bool Buffer_Add(buffer_t *buffer,char element)
{
	uint8_t next_head = buffer->head + 1;

	if(next_head == BUFFER_SIZE)
		next_head = 0;
	
	if(next_head == buffer->tail)
	{
		// buffer full
		return false;
	}

	buffer->data[buffer->head] = element;
	buffer->head = next_head;
	return true;
}

bool Buffer_Get(buffer_t *buffer, char *element)
{
	if(buffer->head == buffer->tail)
	{
		// buffer empty
		return false;
	}

	*element = buffer->data[buffer->tail];
	buffer->tail++;

	if(buffer->tail == BUFFER_SIZE)
		buffer->tail = 0;

	return true;
}
