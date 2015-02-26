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
void TaskStartIFS(void *pvParameters) // Main Red LED Flash
{
	uint8_t TArray[9];
	uint8_t transcieverMsg[2];
	uint8_t grossDataTPA81[18];

	for (uint8_t i = 0x00; i < 0x09; i++){
		transcieverMsg[0] = 0xD0;
		transcieverMsg[1] = i;
		transcieverMsg[2] = 0x20;

		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 3);
		avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Start \n"));

		transcieverMsg[0] = 0xD0;
		transcieverMsg[1] = i + 1;
		I2C_Master_Start_Transceiver_With_Data(&transcieverMsg[0], 2);
		avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Restart \n"));

		uint8_t ok = I2C_Master_Get_Data_From_Transceiver(&grossDataTPA81[2*i], 2);
		avrSerialPrint_P(PSTR("\r\nTaskReadTemperature: Read \n"));

		if (ok != 1){
			avrSerialPrint_P(PSTR("\r\nCouldn't read data \n"));
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
			I2C_Check_Free_After_Stop();
			uint8_t arguments[3] = {0xD0, 0x00, 0x20};
			avrSerialPrint_P(PSTR("INtiliasing arguments \n"));
			avrSerialPrint_P(PSTR("Send write arguments \n"));
			I2C_Master_Start_Transceiver_With_Data(arguments, 3);


		while(1)
		{

		avrSerialPrint_P(PSTR("Intiliasing read arguments \n"));
		uint8_t readArguments[2] = {0xD0, 0x01};
		avrSerialPrint_P(PSTR("Sending write2 sequence \n"));
		I2C_Check_Free_After_Stop();
		I2C_Master_Start_Transceiver_With_Data(readArguments, 2);
		avrSerialPrint_P(PSTR("Sending read arguments \n"));
		uint8_t test[2] = {0xD1, 0x05};
		I2C_Check_Free_After_Stop();
		I2C_Master_Start_Transceiver_With_Data(test, 2);
		uint8_t wheretoread[1] = {0x05};
		avrSerialPrint_P(PSTR("Attempt to read data from thingy \n"));
<<<<<<< HEAD
		I2C_Master_Get_Data_From_Transceiver(&message, 24);
		avrSerialPrint_P(message);
=======
		I2C_Master_Get_Data_From_Transceiver(wheretoread, 1);
		avrSerialPrint_P(0x05);
>>>>>>> 3754c1a74ff3c6388b4f4194d4e00d6bf8b82916
		avrSerialPrint_P(PSTR("message \n"));
		}*/
}
int main(void)
{
    // turn on the serial port for debugging or for other USART reasons.
	taskDISABLE_INTERRUPTS()
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));

	I2C_Master_Initialise(0xD3);
	taskENABLE_INTERRUPTS();
	//uint8_t *arrayPointer = TArray;
	xTaskCreate(
			TaskStartIFS
			,  (const portCHAR *)"IFS"
			,  256
			,  NULL
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

