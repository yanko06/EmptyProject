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
    (void) pvParameters;;
	taskDISABLE_INTERRUPTS();

    TickType_t xLastWakeTime;
	/* Variable used in vTaskDelayUntil. Needs to be set once to allow for a valid start
		time for the vTaskDelayUntil. After this point, vTaskDelayUntil handles itself. */
	xLastWakeTime = xTaskGetTickCount();

	I2C_Master_Initialise(0xD4);
		taskENABLE_INTERRUPTS();
		while(1)
		{
		avrSerialPrint_P(PSTR("Entering Loop \n"));
		uint8_t arguments[3] = {0xD0, 0x00, 0x20};
		avrSerialPrint_P(PSTR("INtiliasing arguments \n"));
		avrSerialPrint_P(PSTR("Send write arguments \n"));
		I2C_Master_Start_Transceiver_With_Data(arguments, 3);
		avrSerialPrint_P(PSTR("Intiliasing read arguments \n"));
		uint8_t readArguments[2] = {0xD0, 0x01};
		avrSerialPrint_P(PSTR("Sending write2 sequence \n"));
		I2C_Master_Start_Transceiver_With_Data(readArguments, 2);
		avrSerialPrint_P(PSTR("Sending read arguments \n"));
		I2C_Master_Start_Transceiver_With_Data(0XD1, 1);
		uint8_t message;
		avrSerialPrint_P(PSTR("Attempt to read data from thingy \n"));
		I2C_Master_Get_Data_From_Transceiver(&message, 24);
		avrSerialPrint_P(PSTR("message \n"));
		}

}
int main(void)
{
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));

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

