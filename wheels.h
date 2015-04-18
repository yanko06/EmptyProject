#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "motion.h"
#include "FreeRTOS.h"


void start_Engine(void);
void setSpeed(int leftSpeed, int rightSpeed);
void validateSpeed();
void calculateNewSpeed(int Side, int latestTickCount);

void initFoward();
void initBackward();
void initTurnLeft();
void initTurnRight();

int getSpeed();
int getDistance();
void rotateCentralServoLeft();
void rotateCentralServoRight();
