#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "I2C/i2cmaster.h"
#include "I2C/i2c_lcd.h"
#include "KEYPAD/keypad.h"
#include "uart.h"
#include "Pulse.h"



char Hrate[3];

int HEART_RATE  = show_heart_rate();

sprintf(Hrate,"%d",HEART_RATE);
if((60 > HEART_RATE) || (HEART_RATE > 120))
{
	valve(2);
	if(60 > HEART_RATE)
	{
		lcd_puts_at("->  LowBPM",6,0);
		lcd_puts_at(Hrate,0,0);
		UART_TxChar('L');
	}
	else if(120 < HEART_RATE)
	{
		lcd_puts_at("->  HighBPM",6,0);
		lcd_puts_at(Hrate,0,0);
		UART_TxChar('H');
	}
	while(1)
	{
		lcd_puts_at("Reset System ?",0,1);
		PORTD |= (1<<PIND6);
		_delay_ms(500);
		PORTD &= ~(1<<PIND6);
		_delay_ms(500);
		while(isPressed(BTN_SET_RESET))
		{
			UART_TxChar('P');
			resetChip();
		}
	}
}
