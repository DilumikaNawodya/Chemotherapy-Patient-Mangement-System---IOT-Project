/*
 * Author: Vassilis Serasidis
 *
 * Home: http://www.serasidis.gr
 * e-mail: avrsite@yahoo.gr, info@serasidis.gr 
 *
 * Devices was used:
 * - GY26:     I2C digital compass.
 * - LIS302DL: 3-axes I2C/SPI accelerometer.
 * - PCF8574T: I2C-to-parallel converter. Usually used to drive dot matrix LCDs such as 16x2 or 20x4 characters.
 * - DS1307:   I2C Real-Time-Clock. 
 * - 24C32:    I2C 32kbit eeprom (4 kByte x 8 bit = 32 kBits). 
 *
 *-----------------------Pulse Sensor---------------------------
 */ 
#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "string.h"

