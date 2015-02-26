////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////    main.c
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<stdio.h>
#include <avr/io.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "i2cMultiMaster.h"

/* serial interface include file. */
#include "serial.h"

#include "wifi.h"


/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));
void TaskStartIFS(uint8_t * TArray) // Main Red LED Flash
{
	while(1){
	uint8_t transcieverMsg[2];
	uint8_t grossDataTPA81[18];

	for (uint8_t i = 0x00; i < 0x09; i++){
		transcieverMsg[0] = 0xD0;
		transcieverMsg[1] = i + 1;

		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 2);
		avrSerialPrint_P(PSTR("TaskReadTemperature: Start \n"));

		transcieverMsg[0] = 0x01;
		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 1);
		avrSerialPrint_P(PSTR("TaskReadTemperature: Restart \n"));

		uint8_t ok = I2C_Master_Get_Data_From_Transceiver(&grossDataTPA81[2*1], (uint8_t) 2);
		avrSerialPrint_P(PSTR("TaskReadTemperature: Read \n"));

		if (ok != 1){
			avrSerialPrint_P(PSTR("Couldn't read data \n"));
		}
	}

	for (int j = 0; j < 18; j++){
		if (j%2 != 0){
			TArray[j/2] = grossDataTPA81[j];
		}
	}

	for (int k = 0; k < 9; k++){
		avrSerialPrintf_P(PSTR("%d"), TArray[k]);
	}


    //(void) pvParameters;;
	//taskDISABLE_INTERRUPTS();

    //TickType_t xLastWakeTime;
	/* Variable used in vTaskDelayUntil. Needs to be set once to allow for a valid start
		time for the vTaskDelayUntil. After this point, vTaskDelayUntil handles itself. */
	/**xLastWakeTime = xTaskGetTickCount();
	I2C_Master_Initialise(0xD0);
			taskENABLE_INTERRUPTS();
			avrSerialPrint_P(PSTR("Entering Loop \n"));
			uint8_t arguments[3] = {0xD0, 0x00, 0x20};
			avrSerialPrint_P(PSTR("INtiliasing arguments \n"));
			avrSerialPrint_P(PSTR("Send write arguments \n"));
			I2C_Master_Start_Transceiver_With_Data(arguments, 2);


		while(1)
		{

		avrSerialPrint_P(PSTR("Intiliasing read arguments \n"));
		uint8_t readArguments[2] = {0xD0, 0x01};
		avrSerialPrint_P(PSTR("Sending write2 sequence \n"));
		I2C_Master_Start_Transceiver_With_Data(readArguments, 1);
		avrSerialPrint_P(PSTR("Sending read arguments \n"));
		uint8_t test[1] = {0xD1};
		I2C_Master_Start_Transceiver_With_Data(test, 0);
		uint8_t message;
		avrSerialPrint_P(PSTR("Attempt to read data from thingy \n"));
		I2C_Master_Get_Data_From_Transceiver(&message, 24);
		avrSerialPrint_P(message);
		avrSerialPrint_P(PSTR("message \n"));
		}*/

}
}
int main(void)
{
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));

	uint8_t TArray[9];
	//uint8_t *arrayPointer = TArray;
	xTaskCreate(
			TaskStartIFS
			,  (const portCHAR *)"IFS"
			,  256
			,  &TArray
			,  3
			,  NULL );


vTaskStartScheduler();
}

/*-----------------------------------------------------------*/


void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    portCHAR *pcTaskName )
{

	DDRB  |= _BV(DDB7);
	PORTB |= _BV(PORTB7);       // main (red PB7) LED on. Mega main LED on and die.
	while(1);
}

/*-----------------------------------------------------------*/

