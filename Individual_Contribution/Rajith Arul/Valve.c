#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>



void valve(int state)
{
	DDRD |= (1<<PIND2);
	int i;
	if(state==2){
		for(i=0;i<100;i++){
			PORTD |= (1<<PIND2);
			_delay_ms(2);
			PORTD &= ~(1<<PIND2);
			_delay_ms(18);
		}
	}
	else if(state==1){
		for(i=0;i<100;i++){
			PORTD |= (1<<PIND2);
			_delay_ms(1);
			PORTD &= ~(1<<PIND2);
			_delay_ms(19);
		}
	}
}


void main()
{
   
   char a;
   char buffer[10];


   DDRB |= (1<<PINB0);
   DDRB |= (1<<PINB1);
   DDRB &= ~(1<<PINB2)

   PORTB &= ~(1<<PINB0);
   PORTB &= ~(1<<PINB1);
  

   lcd_init(LCD_BACKLIGHT_OFF);
   lcd_clear();
   lcd_goto_xy(0,0);
   UART_init(9600);
   


   valve(1); // To open valve
   valve(2); // To close valve



   while(1){
		   a=UART_RxChar();
		   if(a=='L'){
			   lcd_goto_xy(0,0);
			   lcd_puts_at("LOW BPM",0,0);
			   PORTB |= (1<<PINB0);
			   PORTB |= (1<<PINB1);
		   }
		   else if(a=='H'){
			   lcd_goto_xy(0,0);
			   lcd_puts_at("HIGH  BPM",0,0);
			   PORTB |= (1<<PINB0);
			   PORTB |= (1<<PINB1);
		   }
		   else if(a=='S'){
			   lcd_goto_xy(0,0);
			   lcd_puts_at("Speed Varied",0,0);
			   PORTB |= (1<<PINB0);
			   PORTB |= (1<<PINB1);
			}
			else if(a=='C'){
			   lcd_init(LCD_BACKLIGHT_ON);
			   lcd_goto_xy(0,0);
			   lcd_puts_at(" Connect",0,0);
		   }
		   else if(a=='O'){
			   lcd_goto_xy(0,0);
			   lcd_puts_at("Done !!",0,0);
			   int i=0;
		   }
		   else if(a=='P'){
			   resetChip();
		   }

		   if(PIND & (1<<PD0) == 4):
		   {
		   		PORTB &= ~(1<<PINB0);
   				PORTB &= ~(1<<PINB1);
		   }
   }
}
