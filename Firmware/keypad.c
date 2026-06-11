#include <stm32f10x.h>
#include "utils.h"

//Keypad Decleartion
int8_t keypad_scan();

int8_t keypad_scan(){
	uint8_t row[]={0x0e, 0x0d, 0x0b, 0x07};
	
	//0xf used for upper 4bits to keep the pull-ups activated
	uint8_t key_labels[4][4] = {
		{'7','8','9','/'},
		{'4','5','6','*'},
		{'1','2','3','-'},
		{'c','0','=','+'}
	};
	
	int8_t pressed_key;
	uint8_t c;
	uint8_t r;
	c=4;
	pressed_key= -1;
	
	for (r=0; r<4; r++){
		GPIOC->ODR &=0x00f0;
		GPIOC->ODR |= row[r];
		delay_us(2);		
		
		if ((GPIOC->IDR & (1<<4)) == 0){
			c = 0;
			while((GPIOC->IDR & (1<<4)) == 0);
		}
		
		if ((GPIOC->IDR & (1<<5)) == 0){
			c = 1;
			while((GPIOC->IDR & (1<<5)) == 0);
		}
		
		if ((GPIOC->IDR & (1<<6)) == 0){
			c = 2;
			while((GPIOC->IDR & (1<<6)) == 0);
		}
		
		if ((GPIOC->IDR & (1<<7)) == 0){
			c = 3;
			while((GPIOC->IDR & (1<<7)) == 0);
		}
		
		if(c!=4){
			pressed_key = key_labels [r][c];
			return pressed_key;
		}
	}//end of for
	
	return pressed_key; //returns -1 to show no key is pressed
}