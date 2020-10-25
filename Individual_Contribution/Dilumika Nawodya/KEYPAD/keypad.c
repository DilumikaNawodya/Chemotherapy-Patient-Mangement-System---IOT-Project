/*
 * keypad.c
 *
 * Created: 2019-07-28 11:47:57 AM
 *  Author: Dilumika
 */ 

#include "keypad.h"

int keypad_get_number(Keypad key)
{
	switch(key)
	{
		case BTN_0: return 0;break;
		case BTN_1: return 1;break;
		case BTN_2: return 2;break;
		case BTN_3: return 3;break;
		case BTN_4: return 4;break;
		case BTN_5: return 5;break;
		case BTN_6: return 6;break;
		case BTN_7: return 7;break;
		case BTN_8: return 8;break;
		case BTN_9: return 9;break;
		default: return -1;break;
	}
}

void keypad_init(void) {
	DDRB = 0xFF;
	DDRB = 0xF0;//taking column pins as input and row pins as output
	_delay_ms(1);
	PORTB = 0x0F;// powering the row ins
	_delay_ms(1);
}

Keypad waitForKeyPress() {
	register Keypad keypressed = BTN_NO_PRESS;
	while (keypressed == BTN_NO_PRESS) {
		if (PINB != 0b11110000)//in any of column pins goes high execute the loop
		{
			//_delay_ms(5);
			keypressed = PINB;//taking the column value into integer
			DDRB ^= 0b11111111;//making rows as inputs and columns as output
			_delay_ms(1);
			PORTB ^= 0b11111111;//powering columns
			_delay_ms(1);
			keypressed = ~(keypressed | PINB); //taking row value and OR ing it to column value
			//Similar to //kepressed |= PIND; keypressed ^= 0xFF;
		}
		//keypressed = 0;//after showing integer erasing the row column memory
		DDRB ^= 0b11111111;//shifting input and power port
		PORTB ^= 0b11111111;//powering row pins of keypad
		_delay_ms(220);
	}
	return keypressed;
}


bool isPressed(Keypad key) {
	register Keypad keypressed = BTN_NO_PRESS;
	if (PINB != 0b11110000)//in any of column pins goes high execute the loop
	{
		//_delay_ms(5);
		keypressed = PINB;//taking the column value into integer
		DDRB ^= 0b11111111;//making rows as inputs and columns as output
		_delay_ms(1);
		PORTB ^= 0b11111111;//powering columns
		_delay_ms(1);
		keypressed = ~(keypressed | PINB); //taking row value and OR ing it to column value
		//Similar to //kepressed |= PIND; keypressed ^= 0xFF;
	}
	//keypressed = 0;//after showing integer erasing the row column memory
	DDRB ^= 0b11111111;//shifting input and power port
	PORTB ^= 0b11111111;//powering row pins of keypad

	return keypressed == key;
}