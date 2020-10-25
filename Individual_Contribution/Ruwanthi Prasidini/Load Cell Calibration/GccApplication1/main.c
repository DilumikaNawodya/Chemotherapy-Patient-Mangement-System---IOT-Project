//hx711 lib example

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//include uart
#define UART_BAUD_RATE 4800
#include "hx711.h"
#include "i2cmaster.h"
#include "i2c_lcd.h"


//defines running modes
#define HX711_MODERUNNING 1
#define HX711_MODECALIBRATION1OF2 2
#define HX711_MODECALIBRATION2OF2 3

#define HX711_MODECURRENT HX711_MODERUNNING

//2 step calibration procedure
//set the mode to calibration step 1
//read the calibration offset leaving the load cell with no weight
//set the offset to value read
//put a know weight on the load cell and set calibrationweight value
//run the calibration stes 2 of 2
//read the calibration scale
//set the scale to value read

//set the gain
int8_t gain = HX711_GAINCHANNELA128;

#if HX711_MODECURRENT == HX711_MODERUNNING
	//set the offset
	int32_t offset = 8390060;
	//set the scale
	double scale = 3133.353;
#elif HX711_MODECURRENT == HX711_MODECALIBRATION1OF2
	//set the offset
	int32_t offset = HX711_OFFSETDEFAULT;
	//set the scale
	double scale = HX711_SCALEDEFAULT;
#elif HX711_MODECURRENT == HX711_MODECALIBRATION2OF2
	//set the offset
	int32_t offset = 8390060;
	//set the scale
	double scale = HX711_SCALEDEFAULT;
	//set the calibration weight
	double calibrationweight = 0.495;
#endif


int main(void) {
    //init uart
	lcd_init(LCD_BACKLIGHT_ON);
	
	sei();

	char printbuff[100];

	//init hx711
	hx711_init(gain, scale, offset);


#if HX711_MODECURRENT == HX711_MODERUNNING
	for(;;) {
		//get read
		uint32_t read = hx711_read();
		ltoa(read, printbuff, 10);
		lcd_puts_at("Read: ",0,0); lcd_puts_at(printbuff,6,0);

		//get weight
		double weight = hx711_getweight();
		dtostrf(weight, 3, 2, printbuff);
		lcd_puts_at("Weight: ",0,1); lcd_puts_at(printbuff,6,1); lcd_puts_at("kg",13,1);

		_delay_ms(1000);
	}

#elif HX711_MODECURRENT == HX711_MODECALIBRATION1OF2
	for(;;) {
		//get calibration offset
		int32_t calibrationoffset = hx711_calibrate1getoffset();
		ltoa(calibrationoffset, printbuff, 10);
		lcd_puts_at("Calibration offset: ",0,0); lcd_puts_at(printbuff,0,1);

		//_delay_ms(1000);
	}

#elif HX711_MODECURRENT == HX711_MODECALIBRATION2OF2
	for(;;) {
		//calibrate
		hx711_calibrate2setscale(calibrationweight);

		//get scale
		double scale = hx711_getscale();
		dtostrf(scale, 3, 3, printbuff);
		lcd_puts_at("Calibration scale: ",0,0); lcd_puts_at(printbuff,0,1);

		_delay_ms(1000);
	}
#endif
}
