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
#define ROUGE 0
#define BLEU 1
#define VERT 2

void turnOnLED(int color);
void turnOffLED(int color);
void turnOffAllLED();
