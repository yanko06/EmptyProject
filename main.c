////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////    main.c
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

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

#include "wifi.h"


/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerial1Port;
extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));

void TaskPrintToUsart(void *pvParameters) // Main Red LED Flash
{
	TickType_t xLastWakeTime;
/* Variable used in vTaskDelayUntil. Needs to be set once to allow for a valid start
	time for the vTaskDelayUntil. After this point, vTaskDelayUntil handles itself. */
	xLastWakeTime = xTaskGetTickCount();
	xSerialPort = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	char * txt = "124";
	//char pointerTo = &txt;
	uint8_t * cvn = (uint8_t *) txt;
	xComPortHandlePtr xSerialPortPtr = &xSerial1Port;


    while(1)
    {
    	xSerialFlush(xSerialPortPtr);
    	xSerialxPrint(xSerialPortPtr, cvn);

    	//avrSerialPrint(cvn);
		vTaskDelayUntil( &xLastWakeTime, ( 1750 / portTICK_PERIOD_MS ) );
    }
}

int main(void)
{
	/*taskDISABLE_INTERRUPTS();
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));
	char * txt = "Hello";
	uint8_t * cvn = (uint8_t *) txt;
	xComPortHandlePtr xSerialPortPtr = &xSerialPort;

	xSerialFlush(xSerialPortPtr);
	xSerialxPrint(xSerialPortPtr, cvn);
	avrSerialPrint_P(PSTR("Initialising Master \n"));
	I2C_Master_Initialise(0xD4);
	taskENABLE_INTERRUPTS();*/

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

