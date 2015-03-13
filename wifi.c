/*
 * wifi.c
 *
 *  Created on: 2015-01-29
 *      Author: Haffaf
 */

#include "wifi.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <avr/io.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* serial interface include file. */
#include "serial.h"

extern int average;

#define ROUGE 0
#define BLEU 1
#define VERT 2

void turnOnLED(int color){
	if(color==ROUGE){
		DDRH = _BV(DDH3);
		PORTH &= ~_BV(PORTH3);
	}else if(color==BLEU){
		DDRE = _BV(DDE5);
		PORTE &= ~_BV(PORTE5);
	}else if(color==VERT){
		DDRE = _BV(DDE3);
		PORTE &= ~_BV(PORTE3);
	}
};

void turnOffLED(int color){
	if(color==ROUGE){
		DDRH = _BV(DDH3);
		PORTH |=  _BV(PORTH3);
	}else if(color==BLEU){
		DDRE = _BV(DDE5);
		PORTE |= _BV(PORTE5);
	}else if(color==VERT){
		DDRE = _BV(DDE3);
		PORTE |= _BV(PORTE3);
	}
};
void ActivateLED(void *pvParameters)
{

	if ((int) average < 20){
				turnOffLED(0);
				turnOffLED(2);
				turnOnLED(1);
		}
		else if (average < 30){
				turnOffLED(0);
				turnOffLED(1);
				turnOnLED(2);
		}
		else{
				turnOffLED(1);
				turnOffLED(2);
				turnOnLED(0);
		}
};
