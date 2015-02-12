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

/* serial interface include file. */
#include "serial.h"

#include "wifi.h"


/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));

int main(void)
{
	taskDISABLE_INTERRUPTS();
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	I2C_Master_Initialise(0xD4);
	taskENABLE_INTERRUPTS();
	while(1)
	{
	uint8_t arguments[3] = {0xD0, 0x00, 0x20};
	I2C_Master_Start_Transceiver_With_Data(arguments, 3);
	uint8_t readArguments[2] = {0xD0, 0x01};
	I2C_Master_Start_Transceiver_With_Data(readArguments, 2);
	I2C_Master_Start_Transceiver_With_Data(0XD1, 1);
	uint8_t message;
	I2C_Master_Get_Data_From_Transceiver(&message);
	xSerialxPrintf_P(PSTR("message"), uxTaskGetStackHighWaterMark(NULL));
	}


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

