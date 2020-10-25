#ifndef ADC				/* Define library H file if not defined */
#define ADC

#define F_CPU 8000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <util/delay.h>
	
void ADC_Init();
int ADC_Read(char);

#endif