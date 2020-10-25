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

////////////////////////////////// Variables Declared Here //////////////////////////////////////////////
char A = 'A';
char buffer[5];
char a[100];
int Hrate;
double InitialWeight;

int Sec=0;
volatile unsigned long sec;
volatile unsigned long hour=0,minite=0,se=0;
char Ttime[2];
char TTime[5];

char choice = 'k';
int Number = 0;
int entNum = 0;
char vari[5];
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////System Init////////////////////////////////////////////////////////////

void SystemInit()
{
	ADC_Init();
	keypad_init();
	DDRD |= (1<<PIND7) | (1<<PIND6) | (1<<PIND5);
	PORTD &= (1<<PIND7) | (1<<PIND6) | (1<<PIND5);
	lcd_init(LCD_BACKLIGHT_OFF);
	UART_init(9600);
	hx711_init(128, 3133.353, 8390060);
}
void resetChip()
{
	WDTCR = 0x18;
	WDTCR = 0x08;
	while(1);
}
void Clock()
{
		if(sec==60)
		{
			minite++;
			sec = 0;
			lcd_clear();
			if(minite==60)
			{
				hour++;
				minite = 0;
				lcd_clear();
			}
		}
		lcd_puts_at("Time : ",0,0);
		lcd_puts_at("BPM",9,1);
		itoa(hour,Ttime,10);
		lcd_puts_at(Ttime,7,0);
		lcd_puts_at(":",9,0);
		itoa(minite,Ttime,10);
		lcd_puts_at(Ttime,10,0);
		lcd_puts_at(":",12,0);
		itoa(sec,Ttime,10);
		lcd_puts_at(Ttime,13,0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////Weight ~~~~ Hrate ~~~~~ Valve////////////////////////////////////////////////////////
/*
int show_heart_rate()
{
		
		const double sampling_rate = 0.1000 ;     
		const int time_limit = 10 ;      
		const int se=time_limit/(sampling_rate*2);

		int data_points[se+1];
		char val[10];
		data_points[0]=0;
		int thresh=300;
		int count=0;
		prevx=0;
		num=0;
		int count2=0;
		
		for(int i=0;i<time_limit/(sampling_rate*2);i++){
			data_points[i+1]=ADC_Read(0);
			if(i==0)
			{
				num=data_points[i+1];
			}
			itoa(data_points[i+1],val,10);
			int k=16*(data_points[i+1]-200)/375;
			
			itoa(k,val,10);
			_delay_ms(sampling_rate*1000);
		}
		pulse_regression(data_points,se);
		for(int i = 0;i<time_limit/(sampling_rate*2);i++){
			thresh=(i+2)* m + c;

			int a=data_points[i+1];//current value
			char temp[11]="";
			char ccount[3];
			
			if(a>thresh){
				count+=1; //peak counting]
				if(a>data_points[i] && a>data_points[i+2]){
					count2+=1;
				}
				
			}
		}
		int x = count2*(60/time_limit);
		x = valueToBPM(x);
		return x;	
}
*/


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


//////////////////////////////// Main Function///////////////////////////////////////////////////////////

int main(void)
{	
	PORTC |= (1<<PINC3);
	SystemInit();
	InitialWeight = hx711_getweight();
	

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
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	//////////////////////////////////////////////////// Basic Interface /////////////////////////////////////////////
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	_delay_ms(2000);
	UART_TxChar('C');
	
	lcd_clear();
	lcd_puts_at("Measuring Pulse",0,1);
	lcd_puts_at("Measuring Weight",0,0);

	_delay_ms(3000);
	
	lcd_clear();
	int i,j;
	for(i=0,j=15;i<16,j>-1;i++,j--)
	{
		lcd_puts_at(">",i,0);
		lcd_puts_at("<",j,1);
		_delay_ms(100);
	}
	_delay_ms(1000);
	
	lcd_clear();
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	//////////////////////////////////////////////////// Treatment Check /////////////////////////////////////////////
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

	while(A=='B')
	{
		
		dtostrf(hx711_getweight(), 3, 2, a);
		lcd_puts_at("Weight: ",0,1); lcd_puts_at(a,6,1); lcd_puts_at("kg",13,1);
		
		sprintf(buffer,"%d",show_heart_rate());
		lcd_puts_at("Hrate:",0,0);
		lcd_puts_at(buffer,7,0);
		lcd_puts_at("BPM",12,0);

		
		_delay_ms(3000);
		lcd_clear();
		lcd_puts_at("Ready For",4,0);
		lcd_puts_at("Medication",3,1);
		_delay_ms(2000);
		lcd_clear();

		_delay_ms(2000);
		lcd_clear();
		while(1)
		{
			lcd_puts_at("Press Button For",0,0);
			lcd_puts_at("Treatment",4,1);
			if(isPressed(BTN_TREAT))
			{	
				PORTD |= (1<<PIND6);
				A = 'C';
				break;
			}
			else if(isPressed(BTN_SET_RESET))
			{
				lcd_init(LCD_BACKLIGHT_OFF);
				PORTD &= ~(1<<PIND5);
				for(int i=0;i<10;i++)		
				{
					PORTD |= (1<<PIND7);
					_delay_ms(500);	
					PORTD &= ~(1<<PIND7);
					_delay_ms(500);
				}
				resetChip();
			}
		}
		lcd_clear();
	}
	
	PORTD &= ~(1<<PIND7);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	valve(1);   // Valve Will open firstly in there
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	//////////////////////////////////////////////////// Menu //////////////////////////////////////////////////////
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	int HEART_RATE;
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
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	//////////////////////////////////////////////////// Manual //////////////////////////////////////////////////////
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	
		char Hrate[3];
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
					PORTD &= ~(1<<PIND6);
					UART_TxChar('P');
					resetChip();
				}
			}
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
		
		
		if(weightVarience(hx711_getweight())==2)// Here must defined the mass of the constant according to the calibration
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
				if(Sec%10==0){
					lcd_puts_at(Hrate,5,1);
					//UART_sendString(Hrate);
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
		_delay_ms(3000);
		lcd_clear();
	}
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	//////////////////////////////////////////////////// Analog //////////////////////////////////////////////////////
	
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
	
	HEART_RATE  = show_heart_rate();
	while(1)
	{
			char Hrate[3];
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
		
		if(weightVarience(hx711_getweight())==2)// Here must defined the mass of the constant according to the calibration
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
				if(Sec%10==0){
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
		_delay_ms(3000);
		lcd_clear();
	}
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//



 
