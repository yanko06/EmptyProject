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

/* serial interface include file. */
#include "serial.h"
extern uint8_t TArray[9];
void TaskStartIFS(void *pvParameters) // Main Red LED Flash
{
	uint8_t transcieverMsg[2];
	uint8_t grossDataTPA81[18];

		for (uint8_t i = 0x00; i <0x09; i++)
		{
		transcieverMsg[0] = 0xD0;
		transcieverMsg[1] = 0x00;
		transcieverMsg[2] = 0x20;

		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 3);
		avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Start \n"));

		transcieverMsg[0] = 0xD0;
		transcieverMsg[1] = 0x01 + i ;
		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 2);
		//avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Restart \n"));
		transcieverMsg[0] = 0xD1;
		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 1);
		uint8_t ok = I2C_Master_Get_Data_From_Transceiver(&grossDataTPA81[2 * i], 2);
		//avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Read \n"));

		if (ok != 1){
			avrSerialPrint_P(PSTR("\r\nCouldn't read data \n"));
		}

		}
	for (int j = 0; j < 18; j++){
		if (j%2 != 0){
			TArray[j/2] = grossDataTPA81[j];
		}
	}
}


