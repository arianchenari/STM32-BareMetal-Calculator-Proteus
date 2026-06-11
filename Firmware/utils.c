#include "utils.h"

void delay_us(uint16_t t){
	volatile unsigned long l = 0;
	for(uint16_t i = 0; i < t; i++)
	for(l = 0; l < 6; l++)
	{}
}

void delay_ms(uint16_t t){
	volatile unsigned long l = 0;
	for(uint16_t i = 0; i < t; i++)
	for(l = 0; l < 300; l++)
	{}
}