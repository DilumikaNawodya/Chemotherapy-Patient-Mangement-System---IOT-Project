#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "I2C/i2cmaster.h"
#include "HX/hx711.h"
#include "I2C/i2c_lcd.h"



double InitialWeight;

int weightVarience(double weight)
{
	double varience = (InitialWeight - weight);
	InitialWeight = weight;
	if(varience > 0.010)
		return 1;
	else if(varience==0)
		return 2;
	else
		return 3;	
}

if(weightVarience(hx711_getweight())==1)
{
	valve(2);
	lcd_puts_at("Speed Varied",0,0);
	UART_TxChar('S');
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
		
if(weightVarience(hx711_getweight())==2)
{
	lcd_clear();
	lcd_puts_at("CHEMOTHEREPY",2,0);
	lcd_puts_at("FLUID OVER",3,1);
			
	_delay_ms(5000);
	valve(2);
	UART_TxChar('O');
	lcd_clear();
	while(Sec<120)
	{
		itoa(show_heart_rate(),Hrate,10);
		if(Sec%10==0)
		{
			lcd_puts_at(Hrate,5,1);
			UART_sendString(Hrate);
		}
		sec+=5;
		Sec += 5;
		Clock();
	}
	PORTD &= ~(1<<PIND6);
	PORTD |= (1<<PIND5);
	_delay_ms(2000);
	UART_TxChar('R');
	resetChip();
}



void main(){
	hx711_init(128, 3133.353, 8390060);
	while(1)
		dtostrf(hx711_getweight(), 3, 2, a);
		lcd_puts_at("Weight: ",0,1); 
		lcd_puts_at(a,6,1); 
		lcd_puts_at("kg",13,1);
	}
}
