/*	Author: Andrew Shim
 *  	Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo: https://youtu.be/WKp_e8DkYC0
 * 	Google Drive Folder: https://drive.google.com/drive/folders/1e1M0JKhqcYb0zV5CBXDgzasovp_1rJ7Y?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char col_sel_1 = 0x80;
unsigned char col_val_1 = 0x80;

unsigned char col_sel_2 = 0x01;
unsigned char col_val_2 = 0x01;

unsigned char laser_sel_1 = 0x80;
unsigned char laser_val_1 = 0x80;

unsigned char laser_sel_2 = 0x01;
unsigned char laser_val_2 = 0x01;

uint16_t input_1 = 0x00;
uint16_t input_2 = 0x00;
uint16_t input_3 = 0x00;
uint16_t input_4 = 0x00;

unsigned int blink_1, blink_2 = 0;
unsigned int laser_off = 0;

unsigned int score_1 = 3;
unsigned int score_2 = 3;

void delay_ms(int miliSec) {
    int i, j;
    for(i = 0; i < miliSec; i++)
    for(j = 0; j < 775; j++) {
	asm("nop");
    }
}

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

void SET_PIN(unsigned char pin) {
    ADMUX = (pin <= 0x07) ? pin : ADMUX;
    static unsigned char i;
    for(i = 0; i < 15; i++) {
	asm("nop");
    }
}

enum SM_States_1 {SM_Start_1, SM_DISPLAY_1};
int SM_Tick_1(int state) {
    SET_PIN(0);
    input_1 = ADC;

    switch(state) {
	case SM_Start_1:
	    state = SM_DISPLAY_1;
	    break;
	case SM_DISPLAY_1:
	    state = SM_DISPLAY_1;
	    break;
	default:
	    state = SM_Start_1;
	    break;
    }
   
    switch(state) {
	case SM_DISPLAY_1:
	    if(input_1 < 225) {
		col_val_1 = (col_val_1 <= 0x01) ? 0x01 : col_val_1 >> 1;
	    }
	    if(input_1 > 700) {
		col_val_1 = (col_val_1 >= 0x40) ? 0x80 : col_val_1 << 1;
	    }
	    break;
	default:
	    break;
    }
    return state;
}

enum SM_States_2 {SM_Start_2, SM_DISPLAY_2};
int SM_Tick_2(int state) {
    SET_PIN(1);
    input_2 = ADC;

    switch(state) {
	case SM_Start_2:
	    state = SM_DISPLAY_2;
	    break;
	case SM_DISPLAY_2:
	    state = SM_DISPLAY_2;
	    break;
	default:
	    state = SM_Start_2;
	    break;
    }
   
    switch(state) {
	case SM_DISPLAY_2:
	    if(input_2 > 700) {
		col_sel_1 = (col_sel_1 >= 0x40) ? 0x80 : col_sel_1 << 1;
	    }
	    if(input_2 < 225) {
		col_sel_1 = (col_sel_1 <= 0x10) ? 0x10 : col_sel_1 >> 1;
	    }
	    break;
	default:
	    break;
    }
    return state;
}

enum SM_States_3 {SM_Start_3, SM_DISPLAY_3};
int SM_Tick_3(int state) {
    SET_PIN(2);
    input_3 = ADC;

    switch(state) {
	case SM_Start_3:
	    state = SM_DISPLAY_3;
	    break;
	case SM_DISPLAY_3:
	    state = SM_DISPLAY_3;
	    break; 
	default:
	    state = SM_Start_3;
	    break;
    }
   
    switch(state) {
	case SM_DISPLAY_3:
	    if(input_3 < 225) {
		col_val_2 = (col_val_2 <= 0x01) ? 0x01 : col_val_2 >> 1;
	    }
	    if(input_3 > 700) {
		col_val_2 = (col_val_2 >= 0x40) ? 0x80 : col_val_2 << 1;
	    }
	    break;
	default:
	    break;
    }
    return state;
}

enum SM_States_4 {SM_Start_4, SM_DISPLAY_4};
int SM_Tick_4(int state) {
    SET_PIN(3);
    input_4 = ADC;

    switch(state) {
	case SM_Start_4:
	    state = SM_DISPLAY_4;
	    break;
	case SM_DISPLAY_4:
	    state = SM_DISPLAY_4;
	    break; 
	default:
	    state = SM_Start_4;
	    break;
    }
   
    switch(state) {
	case SM_DISPLAY_4:
	    if(input_4 > 700) {
		col_sel_2 = (col_sel_2 >= 0x04) ? 0x08 : col_sel_2 << 1;
	    }
	    if(input_4 < 225) {
		col_sel_2 = (col_sel_2 <= 0x01) ? 0x01 : col_sel_2 >> 1;
	    }
	    break;
	default:
	    break;
    }
    return state;
}

enum Output_1 {OUT1_Start, OUT1_WAIT, OUT1_BLINK};
int Output_Tick_1(int state) {
    switch(state) {
	case OUT1_Start:
	    score_1 = 3;
	    state = OUT1_WAIT;
	    break;
	case OUT1_WAIT:
    	    if((~PINB & 0x04) == 0x04) {
	        laser_off = 0;
		col_sel_1 = 0x80;
		col_val_1 = 0x80;
		col_sel_2 = 0x01;
		col_val_2 = 0x01;
		blink_1 = 0;
		score_1 = 3;
		score_2 = 3;
	    }
	    if(blink_1 == 1) {
		state = OUT1_BLINK;
	    } else {
	        transmit_data_1(col_sel_1);
	        transmit_data_1(col_sel_1);
	        transmit_data_1(col_sel_1);
	        transmit_data_2(~col_val_1);
	        transmit_data_2(~col_val_1);
	        transmit_data_2(~col_val_1);
	        transmit_data_1(0x00);
	        transmit_data_2(0xFF);
	        state = OUT1_WAIT;
	    }
	    break;
	case OUT1_BLINK:
	    transmit_data_1(0x00);	    
	    transmit_data_2(0xFF);
	    if((~PINB & 0x04) == 0x04) {
	        laser_off = 0;
		col_sel_1 = 0x80;
		col_val_1 = 0x80;
		col_sel_2 = 0x01;
		col_val_2 = 0x01;
		blink_1 = 0;
		score_1 = 3;
		score_2 = 3;
		state = OUT1_WAIT;
	    } else {
		state = OUT1_BLINK;
	    }
	    break;
	default:
	    state = OUT1_Start;
	    break;
    }
    switch(state) {
	case OUT1_WAIT:
	    switch(score_1) {
		case 3:
		    PORTD = (PORTD & 0xC7) | 0x38;
		    break;
		case 2:
		    PORTD = (PORTD & 0xC7) | 0x30;
		    break;
		case 1:
		    PORTD = (PORTD & 0xC7) | 0x20;
		    break;
		case 0:
		    PORTD = PORTD & 0xC7;
		    break;
		default:
	   	    break;
	    }
	    break;
	case OUT1_BLINK:
	    PORTD = PORTD & 0xC7;
	    break;
	default:
	    break;
    }
    return state;
}

enum Output_2 {OUT2_Start, OUT2_WAIT, OUT2_BLINK};
int Output_Tick_2(int state) {
    switch(state) {
	case OUT2_Start:
	    score_2 = 3;
	    state = OUT2_WAIT;
	    break;
	case OUT2_WAIT:
    	    if((~PINB & 0x04) == 0x04) {
	        laser_off = 0;
		col_sel_1 = 0x80;
		col_val_1 = 0x80;
		col_sel_2 = 0x01;
		col_val_2 = 0x01;
		blink_2 = 0;
		score_1 = 3;
		score_2 = 3;
	    }
	    if(blink_2 == 1) {
		state = OUT2_BLINK;
	    } else {
	        transmit_data_1(col_sel_2);
	        transmit_data_1(col_sel_2);
	        transmit_data_1(col_sel_2);
	        transmit_data_2(~col_val_2);
	        transmit_data_2(~col_val_2);
	        transmit_data_2(~col_val_2);
	        transmit_data_1(0x00);
	        transmit_data_2(0xFF);
	        state = OUT2_WAIT;
	    }
	    break;
	case OUT2_BLINK:
	    transmit_data_1(0x00);	    
	    transmit_data_2(0xFF);
	    if((~PINB & 0x04) == 0x04) {
	        laser_off = 0;
		col_sel_1 = 0x80;
		col_val_1 = 0x80;
		col_sel_2 = 0x01;
		col_val_2 = 0x01;
		blink_2 = 0;
		score_1 = 3;
		score_2 = 3;
		state = OUT2_WAIT;
	    } else {
		state = OUT2_BLINK;
	    }
	    break;
	default:
	    state = OUT2_Start;
	    break;
    }
    switch(state) {
	case OUT1_BLINK:
	    PORTD = PORTD & 0xF8;
	    break;
	case OUT2_WAIT:
	    switch(score_2) {
		case 3:
		    PORTD = (PORTD & 0xF8) | 0x07;
		    break;
		case 2:
		    PORTD = (PORTD & 0xF8) | 0x03;
		    break;
		case 1:
		    PORTD = (PORTD & 0xF8) | 0x01;
		    break;
		case 0:
		    PORTD = PORTD & 0xF8;
		    break;
		default:
	   	    break;
	    }
	default:
	    break;
    }
    return state;
}

enum Laser_1 {LASER1_Start, LASER1_WAIT, LASER1_SHOOT, LASER1_OFF};
int LASER_Tick_1(int state) {
    switch(state) {
	case LASER1_Start:
	    state = LASER1_WAIT;
	    break;
	case LASER1_WAIT:
	    laser_sel_1 = col_sel_1;
	    laser_val_1 = col_val_1;
	    if(((~PINB & 0x01) == 0x01) && (laser_off != 1)) {
		state = LASER1_SHOOT;
	    } else {
		state = LASER1_WAIT;
	    }
	    break;
	case LASER1_SHOOT:
	    if(laser_sel_1 >= 0x02) {
		laser_sel_1 = laser_sel_1 >> 1;
		transmit_data_1(laser_sel_1);
		transmit_data_1(laser_sel_1);
		transmit_data_1(laser_sel_1);
		transmit_data_2(~laser_val_1);
		transmit_data_2(~laser_val_1);
		transmit_data_2(~laser_val_1);
	        transmit_data_1(0x00);
	        transmit_data_2(0xFF);
		delay_ms(1);
		if(laser_sel_1 == col_sel_2 && laser_val_1 == col_val_2) { 
		    score_2--;
		    if(score_2 == 0) {
		        blink_2 = 1;
		        laser_off = 1;  
		        state = LASER1_OFF;
		    }
		    state = LASER1_WAIT;
		} else {
		    state = LASER1_SHOOT;
		}
	    } else {
		laser_sel_1 = col_sel_1;
		state = LASER1_WAIT;
	    }
	    break;
	case LASER1_OFF:
	    if(blink_2 == 0) {
		state = LASER1_Start;
	    } else {
		state = LASER1_OFF;
	    }
	    break;
	default:
	    state = LASER1_Start;
	    break;
    }
    return state;
}

enum Laser_2 {LASER2_Start, LASER2_WAIT, LASER2_SHOOT, LASER2_OFF};
int LASER_Tick_2(int state) {
    switch(state) {
	case LASER2_Start:
	    state = LASER2_WAIT;
	    break;
	case LASER2_WAIT:
	    laser_sel_2 = col_sel_2;
	    laser_val_2 = col_val_2;
	    if(((~PINB & 0x02) == 0x02) && (laser_off != 1)) {
		state = LASER2_SHOOT;
	    } else {
		state = LASER2_WAIT;
	    }
	    break;
	case LASER2_SHOOT:
	    if(laser_sel_2 <= 0x40) {
		laser_sel_2 = laser_sel_2 << 1;
		transmit_data_1(laser_sel_2);
		transmit_data_1(laser_sel_2);
		transmit_data_1(laser_sel_2);
		transmit_data_2(~laser_val_2);
		transmit_data_2(~laser_val_2);
		transmit_data_2(~laser_val_2);
	        transmit_data_1(0x00);
	        transmit_data_2(0xFF);
		delay_ms(1);
		if(laser_sel_2 == col_sel_1 && laser_val_2 == col_val_1) { 
		    score_1--;
		    if(score_1 == 0) {
		        blink_1 = 1;
		        laser_off = 1;  
		        state = LASER2_OFF;
		    }
		    state = LASER2_WAIT;
		} else {
		    state = LASER2_SHOOT;
		}
	    } else {
		laser_sel_2 = col_sel_2;
		state = LASER2_WAIT;
	    }
	    break;
	case LASER2_OFF:
	    if(blink_1 == 0) {
		state = LASER2_Start;
	    } else {
		state = LASER2_OFF;
	    }
	    break;
	default:
	    state = LASER2_Start;
	    break;
    }
    return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
    static task task1, task2, task3, task4, task5, task6, task7, task8;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    const char start = -1;

    task1.state = start;
    task1.period = 20;
    task1.elapsedTime = task1.period;
    task1.TickFct = &SM_Tick_1;

    task2.state = start;
    task2.period = 20;
    task2.elapsedTime = task2.period;
    task2.TickFct = &SM_Tick_2;

    task3.state = start;
    task3.period = 1;
    task3.elapsedTime = task3.period;
    task3.TickFct = &Output_Tick_1;

    task4.state = start;
    task4.period = 20;
    task4.elapsedTime = task4.period;
    task4.TickFct = &SM_Tick_3;

    task5.state = start;
    task5.period = 20;
    task5.elapsedTime = task5.period;
    task5.TickFct = &SM_Tick_4;

    task6.state = start;
    task6.period = 1;
    task6.elapsedTime = task6.period;
    task6.TickFct = &Output_Tick_2;

    task7.state = start;
    task7.period = 25;
    task7.elapsedTime = task7.period;
    task7.TickFct = &LASER_Tick_1;

    task8.state = start;
    task8.period = 25;
    task8.elapsedTime = task8.period;
    task8.TickFct = &LASER_Tick_2;

    ADC_init();

    TimerSet(3);
    TimerOn();

    /* Insert your solution below */
    while (1) {
	for (unsigned i = 0; i < numTasks; i++) {
	    if ( tasks[i]->elapsedTime == tasks[i]->period ) {
		tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
		tasks[i]->elapsedTime = 0;
	    }
	    tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag) {}
	TimerFlag = 0;
    }
}
