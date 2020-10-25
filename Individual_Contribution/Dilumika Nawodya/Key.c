#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "I2C/i2cmaster.h"
#include "HX/hx711.h"
#include "I2C/i2c_lcd.h"
#include "KEYPAD/keypad.h"
#include "uart.h"
#include "Pulse.h"


void main()
{
	while(1)
	{
		while(isPressed(BTN_SET_RESET))
		{
			A = 'B';
			lcd_init(LCD_BACKLIGHT_ON);
			PORTD |= (1<<PIND7);
		}
		if(A == 'B')
			break;
	}

	while(1)
	{
		while(1)
		{
			lcd_puts_at("Manual Input",1,0);
			lcd_puts_at("Analog Input",1,1);
			lcd_puts_at(">",0,0);
			choice = 'y';
			while(!isPressed(BTN_OK))
			{
				if(isPressed(BTN_UP))
				{
					lcd_clear();
					lcd_puts_at("Manual Input",1,0);
					lcd_puts_at("Analog Input",1,1);
					lcd_puts_at(">",0,0);
					choice = 'y';
				}
				else if(isPressed(BTN_DOWN))
				{
					lcd_clear();
					lcd_puts_at("Manual Input",1,0);
					lcd_puts_at("Analog Input",1,1);
					lcd_puts_at(">",0,1);
					choice = 'n';
				}
			}
			break;
		}
		
		lcd_clear();
		
		if(choice == 'y')
		{
			while(1)
			{
				lcd_puts_at("1=>Abnormal Rate",0,0);
				lcd_puts_at("2=>Normal Rate",0,1);
				if(isPressed(BTN_1))
				{
					int num = rand()%200;
					if(num<60 || num>120)
						HEART_RATE = num;
					break;
				}
				else if(isPressed(BTN_2))
				{
					HEART_RATE = rand()%60+60;
					break;
				}
			}		
		}
		
	if(choice == 'n')
		break;
			
	lcd_clear();

}