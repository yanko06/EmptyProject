/*
 * IFS.c
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
#include "motion.h"

/* serial interface include file. */
#include "serial.h"
extern uint8_t temperatureTable[9];
extern int average;
extern char *tempPTR;
extern char temperature;
void TaskStartIFS(void *pvParameters) // Main Red LED Flash
{
	uint8_t messagesForSlave[3];
	uint8_t data[18];

	for (uint8_t i = 0x00; i < 0x09; i++) {
		messagesForSlave[0] = 0xD0;
		messagesForSlave[1] = 0x00;
		messagesForSlave[2] = 0x20;

		//Send data to slave to begin reading data.
		I2C_Master_Start_Transceiver_With_Data(&messagesForSlave[0], 3);

		messagesForSlave[0] = 0xD0;
		messagesForSlave[1] = 0x01 + i;
		//start reading from slave.
		I2C_Master_Start_Transceiver_With_Data(&messagesForSlave[0], 2);

		messagesForSlave[0] = 0xD1;
		//restart.
		I2C_Master_Start_Transceiver_With_Data(&messagesForSlave[0], 1);
		I2C_Master_Get_Data_From_Transceiver(&data[2 * i], 2);
		//Put raw data into table.
		for (int j = 0; j < 18; j++) {
			if (j % 2 != 0) {
				temperatureTable[j / 2] = data[j];
			}
		}
	}
}
;
void CalculateTemperatureAverage(void *pvParameters) {
	average = 0;
	for (int i = 1; i < 9; i++) {
		average = average + temperatureTable[i];
	}
	average = average / 8;

	temperature = (char) (average);
	tempPTR = &temperature;
}
;

void rotateCentralServoMotor(void *pvParameters) {
	while (1){
	avrSerialPrint_P(PSTR("\r\n\n\nRotateCentralServoMotor\r\n"));
	motion_servo_set_pulse_width(MOTION_SERVO_CENTER, 1100);
	motion_servo_start(MOTION_SERVO_CENTER);
	vTaskDelay(( 2000 / portTICK_PERIOD_MS ));
	motion_servo_stop(MOTION_SERVO_CENTER);
	motion_servo_set_pulse_width(MOTION_SERVO_CENTER, 4800);
	motion_servo_start(MOTION_SERVO_CENTER);
	vTaskDelay(( 2000 / portTICK_PERIOD_MS ));
	motion_servo_stop(MOTION_SERVO_CENTER);
	}
}
;

