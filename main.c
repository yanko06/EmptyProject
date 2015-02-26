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
#include "IFS.h"

/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerial1Port;
extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));
int main(void)
{
	/*taskDISABLE_INTERRUPTS();
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));
	char * txt = "Hello";
	uint8_t * cvn = (uint8_t *) txt;
	xComPortHandlePtr xSerialPortPtr = &xSerialPort;

	xTaskCreate(
			TaskStartIFS
			,  (const portCHAR *)"IFS"
			,  256
			,  NULL
			,  3
			,  NULL );

    xTaskCreate(
    		TaskPrintToUsart
		,  (const portCHAR *)"USART"
		,  256				
		,  NULL
		,  3
		,  NULL ); 



	vTaskStartScheduler();

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

