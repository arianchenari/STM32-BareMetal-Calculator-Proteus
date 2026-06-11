#include <stm32f10x.h>
#include "utils.h"
#include "lcd.h"
#include "keypad.h"

//Calculator Function Decleartion
int16_t cal_getop(int16_t, int16_t); // get the operands digit
void cal_set_opn1(int8_t *, int8_t *, uint8_t *, uint8_t *); // set the first operand
int16_t cal_res(int16_t, int16_t, int8_t); // calculate resault
void cal_clear(int16_t *, uint8_t *, int8_t *, int16_t *, int8_t *, int8_t *, int16_t *, uint8_t *); // clear the values
void cal_show_res(int16_t, int16_t, int8_t, int16_t); // show resault on LCD

int main(void){
	RCC->APB2ENR |= 0xFC; // Enable clocks for GPIO
	GPIOA->CRL = 0x44333333; // PA0-PA5 as outputs
	GPIOC->CRL = 0x88883333; // PC0-PC3 as out put, PC4-PC7 as input
	GPIOC->ODR = 0x00FF; // PC5-PC7 as input pull-up
	
	//Welcome
	lcd_init();
	lcd_print("Ready...");
	delay_ms(500);
	lcd_clear();
	
	//Initialize the variables
	int16_t opn1 = 0;
	uint8_t opn1_state = 0; // 0 mean Empty
	int8_t asi_opn1 = 1; // default op1 is positive(1) if we want negativethis flag (-1)
	int16_t opn2 = 0;
	int8_t asi_opn2 = 1; // default op1 is positive(1) if we want negativethis flag (-1)
	int8_t opr;
	int16_t res;
	uint8_t n = 0;
	
	while(1){
		int8_t k = keypad_scan(); // get input user
		
		switch(k){
			case '+': cal_set_opn1(&k, &opr, &opn1_state, &n); break;
			case '-':
				if(!opn1_state && n == 0){// negative opn1
					lcd_sendData('-');
					asi_opn1 = -1;
				}
				else if(opn1_state && n == 0){// negative opn2
					lcd_parentheses();
					asi_opn2 = -1;
				}
				else{// subtraction operator
						cal_set_opn1(&k, &opr, &opn1_state, &n);
				}
				break;
			case '*': cal_set_opn1(&k, &opr, &opn1_state, &n); break;
			case '/': cal_set_opn1(&k, &opr, &opn1_state, &n); break;
			case '=':
				if(asi_opn1 == -1) opn1 = opn1 * asi_opn1;
				if(asi_opn2 == -1) opn2 = opn2 * asi_opn2;
				res = cal_res(opn1, opn2, opr);
				cal_show_res(opn1, opn2, opr, res);
				cal_clear(&opn1, &opn1_state, &asi_opn1, &opn2, &asi_opn2, &opr, &res, &n); // clear values
				break;
			case 'c':cal_clear(&opn1, &opn1_state, &asi_opn1, &opn2, &asi_opn2, &opr, &res, &n); break;
			default:
				if(n > 4){
					lcd_clear();
					lcd_print("Too Digit!!!");
					delay_ms(500);
					cal_clear(&opn1, &opn1_state, &asi_opn1, &opn2, &asi_opn2, &opr, &res, &n);
					n = 0;
					break;
				}
				
				if(k != -1){
					if(!opn1_state) // if first operand is empty
						opn1 = cal_getop(k,opn1);
					else // if second operand is empty
						opn2 = cal_getop(k,opn2);
					n++;
				}
				
		}
	}
	
}

//Calculator Function Definition
int16_t cal_getop(int16_t k, int16_t op){

	int digit = k - '0';
	lcd_sendData(k);
	return ((op*10) + digit);
}
void cal_set_opn1(int8_t *k, int8_t *opr, uint8_t *opn1_state, uint8_t *n){
	lcd_sendData(*k);
	*opr = *k;
	*opn1_state = 1;
	*n = 0;
}
int16_t cal_res(int16_t opn1,int16_t opn2, int8_t opr){
	switch (opr){
		case '+': return (opn1 + opn2);
		case '-': return (opn1 - opn2);			
		case '*': return (opn1 * opn2);
		case '/':
			if(opn2 == 0)
			return 'e';
			else 
			return (opn1 / opn2);
	}
	
	return 'e';
}
void cal_clear(int16_t *opn1, uint8_t *opn1_state, int8_t *asi_opn1, int16_t *opn2, int8_t *asi_opn2, int8_t *opr, int16_t *res, uint8_t *n){
	lcd_clear();
	*opn1 = 0;
	*opn1_state = 0;
	*asi_opn1 = 1;
	*opn2 = 0;
	*asi_opn2 = 1;
	*opr = 0;
	*res = 0;
	*n = 0;
}

void cal_show_res(int16_t opn1, int16_t opn2, int8_t opr, int16_t res){
	lcd_clear();
	lcd_print_int(opn1);
	lcd_sendData(opr);
	
	if(opn2 < 0){
		lcd_sendData('(');
		lcd_print_int(opn2);
		lcd_sendData(')');
	}
	else
	lcd_print_int(opn2);
	
	lcd_sendData('=');
	if(res != 'e')
	lcd_print_int(res);
	else{
		lcd_clear();
		lcd_print("Unvalid input.");
	}
	lcd_gotoxy(1,2);
	lcd_print("Presss any key.");
	
	//waiting for input
	int16_t k;
	do{
		k = keypad_scan();
	}while(k == -1);
	delay_ms(20);
	while(keypad_scan() != -1);
}