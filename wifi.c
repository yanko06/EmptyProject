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

void TaskBlinkRedLED(void *pvParameters) // Main Red LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	/* Variable used in vTaskDelayUntil. Needs to be set once to allow for a valid start
		time for the vTaskDelayUntil. After this point, vTaskDelayUntil handles itself. */
	xLastWakeTime = xTaskGetTickCount();

	DDRH = _BV(DDH3);

    while(1)
    {
    	PORTH &= ~_BV(PORTH3);       // Red LED on.
		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTH |=  _BV(PORTH3);       // Red LED off.
		vTaskDelayUntil( &xLastWakeTime, ( 1750 / portTICK_PERIOD_MS ) );
    }
}

void TaskBlinkBlueLED(void *pvParameters) // Main Blue LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();

	//DDRE = _BV(DDE5);

    while(1)
    {
    	vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_PERIOD_MS ) );
    	DDRE = _BV(DDE5);
    	PORTE &= ~_BV(PORTE5);      // Blue LED on.

		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTE |=  _BV(PORTE5);       // Blue LED off.
		vTaskDelayUntil( &xLastWakeTime, ( 1250 / portTICK_PERIOD_MS ) );

    }
}

void TaskBlinkGreenLED(void *pvParameters) // Main Green LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	
	xLastWakeTime = xTaskGetTickCount();

	//DDRE = _BV(DDE3);

    while(1)
    {
    	vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );
    	DDRE = _BV(DDE3);
    	PORTE &= ~_BV(PORTE3);       // Green LED on.

		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTE |=  _BV(PORTE3);       // Green LED off
		vTaskDelayUntil( &xLastWakeTime, ( 750 / portTICK_PERIOD_MS ) );

    }
}
