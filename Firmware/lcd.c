#include <stm32f10x.h>
#include "utils.h"
#include "lcd.h"

void lcd_init(){
	GPIOA->ODR &= ~(1<<LCD_EN); /* LCD_EN = 0 */
	delay_us(3000); /* wait 3ms */
	
	lcd_sendCommand(0x33); /* send 0x33 for init. */
	lcd_sendCommand(0x32); /* send 0x32 for init. */
	lcd_sendCommand(0x28); /* init. LCD 2 line,5x7 matrix */
	lcd_sendCommand(0x0C); /* display on, cursor off */
	lcd_sendCommand(0x01); /* clear LCD */
	
	delay_us(2000); /* wait 2ms */
	lcd_sendCommand(0x06); /* shift cursor right */
}

void lcd_sendCommand(unsigned char cmd){
	GPIOA->ODR &= ~ (1<<LCD_RS); /* RS = 0 for command */
	lcd_putValue(cmd);
}

void lcd_sendData(unsigned char data){
	GPIOA->BSRR = (1<<LCD_RS); /* RS = 1 for data */
	lcd_putValue(data);
}

void lcd_putValue(unsigned char value){
	GPIOA->BRR = 0x0F; /* clear PA0-PA3 */
	GPIOA->BSRR = (value>>4)&0x0F; /* put high nibble on PA0-PA3 */
	GPIOA->ODR |= (1<<LCD_EN); /* EN = 1 for H-to-L pulse */
	delay_us(1); /* make EN pulse wider */
	GPIOA->ODR &= ~ (1<<LCD_EN);/* EN = 0 for H-to-L pulse */
	delay_us(100); /* wait */
	
	GPIOA->BRR = 0x0F; /* clear PA0-PA3 */
	GPIOA->BSRR = value&0x0F; /* put low nibble on PA0-PA3 */
	GPIOA->ODR |= (1<<LCD_EN); /* EN = 1 for H-to-L pulse */
	delay_us(1); /* make EN pulse wider */
	GPIOA->ODR &= ~(1<<LCD_EN); /* EN = 0 for H-to-L pulse */
	delay_us(100); /* wait */
}
void lcd_gotoxy(unsigned char x, unsigned char y){
	const unsigned char firstCharAdr[]={0x80,0xC0,0x94,0xD4};
	lcd_sendCommand(firstCharAdr[y-1] + x - 1);
	delay_us(100);
}

void lcd_print(char * str){
	unsigned char i = 0;
	
	while(str[i] != 0){ /* while it is not end of string */
		lcd_sendData(str[i]); /* show str[i] on the LCD */
		i++;
	}
}

void lcd_print_int(int num){

	char buf[6];
	int i = 0;
	
	if(num == 0){
		lcd_sendData('0');
		return;
	}

	if(num < 0){
		lcd_sendData('-');
		num = -num;
	}

	while(num > 0){
		buf[i++] = (num % 10) + '0';
		num /= 10;
	}

	while(i--){
		lcd_sendData(buf[i]);
	}
}


void lcd_clear(){
	lcd_sendCommand(0x01); /* clear LCD */
}
void lcd_parentheses(){
	lcd_sendData('(');
	lcd_sendData('-');
	lcd_sendData(' ');
	lcd_sendData(' ');
	lcd_sendData(' ');
	lcd_sendData(' ');
	lcd_sendData(')');
	lcd_sendCommand(0x10); 
	lcd_sendCommand(0x10);	// shifr cursor back to ()
	lcd_sendCommand(0x10); 
	lcd_sendCommand(0x10); 
	lcd_sendCommand(0x10); 
}
