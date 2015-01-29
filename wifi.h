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

void TaskBlinkRedLED(void *pvParameters); // Main Red LED Flash
void TaskBlinkBlueLED(void *pvParameters); // Main Blue LED Flash
void TaskBlinkGreenLED(void *pvParameters); // Main Green LED Flash
