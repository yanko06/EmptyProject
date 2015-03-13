/*
 * IFS.h
 *
 *  Created on: 2015-02-26
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

void TaskStartIFS(uint8_t * TArray);
void CalculateAverage(void *pvParameters);


