#include "print.h"

#include <stdio.h>

void print_buffer(const void *data, uint32_t size)
{
	const uint8_t *d = (const uint8_t *)data;
	uint32_t i;
	for (i = 0; i < size; i += 16)
	{
		uint32_t j;
		printf("%08X |", i);
		for (j = i; j < i + 16; j++)
		{
			if (j < size)
			{
				printf(" %02X", d[j]);
			}
			else
			{
				printf("   ");
			}
		}
		printf(" | ");
		for (j = i; j < i + 16; j++)
		{
			if (j < size)
			{
				putchar((d[j] >= 32) && (d[j] < 128) ? d[j] : '.');
			}
			else
			{
				putchar(' ');
			}
		}
		printf("\n");
	}
}
