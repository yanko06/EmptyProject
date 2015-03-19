#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "motion.h"
#include "FreeRTOS.h"


void start_Engine(void);
void turnAroundAntiClockwise(void);
void turnAroundClockwise(void);
void turnDegree(int degree);
void moveFoward();
void moveBackward();
double getSpeed();
void printSpeed();
