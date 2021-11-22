/*	Author: Andrew Shim
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void transmit_data(unsigned char data) {
    int i;
    for(i = 0; i < 8; i++) {
	PORTC = 0x08;
	PORTC |= ((data >> i) & 0x01);
	PORTC |= 0x02;
    }
    PORTC |= 0x04;
    PORTC = 0x00;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char insertData = 0x66;
    /* Insert your solution below */
    while (1) {
	transmit_data(insertData);
    }
    return 0;
}
