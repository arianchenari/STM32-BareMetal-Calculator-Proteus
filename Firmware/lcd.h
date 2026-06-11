//LCD Declreation and Defines
void lcd_init();
void lcd_sendCommand(unsigned char);
void lcd_sendData(unsigned char);
void lcd_putValue(unsigned char);
void lcd_print(char *);
void lcd_print_int(int);
void lcd_gotoxy(unsigned char , unsigned char);
void lcd_clear();
void lcd_parentheses();

#define LCD_RS 4 // PA4
#define LCD_EN 5 // PA5