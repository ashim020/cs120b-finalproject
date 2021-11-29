/*	Author: Andrew Shim
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo: https://drive.google.com/file/d/1fZNmacXppI1VTbf7vidQJOo7u2VGk6Uh/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char col_sel = 0x10;
unsigned char col_val = 0x10;

void transmit_data_1(unsigned char data) {
    int i;
    for(i = 0; i < 8; i++) {
	PORTC = 0x08;
	PORTC |= ((data >> i) & 0x01);
	PORTC |= 0x02;
    }
    PORTC |= 0x04;
    PORTC = 0x00;
}

void transmit_data_2(unsigned char data) {
    int i;
    for(i = 0; i < 8; i++) {
        PORTC = 0x80;
	PORTC |= (((data >> i) << 4) & 0x10);
	PORTC |= 0x20;
    }
    PORTC |= 0x40;
    PORTC = 0x00;
}

void ADC_init() {
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

enum SM_States {SM_Start, SM_DISPLAY};
int SM_Tick(int state) {
    ADC_init();
    ADMUX = !ADMUX;
    unsigned short input;
    input = ADC;

    switch(state) {
	case SM_Start:
	    state = SM_DISPLAY;
	    break;
	case SM_DISPLAY:
	    state = SM_DISPLAY;
	    break;
	default:
	    state = SM_Start;
	    break;
    }
   
    switch(state) {
	case SM_DISPLAY:
	    if(input > 700 && ADMUX == 0) {
		col_sel = (col_sel >= 0x40) ? 0x80 : col_sel << 1;
	    }
	    if(input < 225 && ADMUX == 1) {
		col_val = (col_val <= 0x01) ? 0x01 : col_val >> 1;
	    }
	    if(input < 225 && ADMUX == 0) {
		col_sel = (col_sel <= 0x01) ? 0x01 : col_sel >> 1;
	    }
	    if(input > 700 && ADMUX == 1) {
		col_val = (col_val >= 0x40) ? 0x80 : col_val << 1;
	    }
	    break;
	default:
	    break;
    }

    transmit_data_1(col_sel);
    transmit_data_2(~col_val);
    
    return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
    TimerSet(50);
    TimerOn();

    /* Insert your solution below */
    while (1) {
	SM_Tick(SM_DISPLAY);
	while(!TimerFlag) {}
	TimerFlag = 0;
    }
}
