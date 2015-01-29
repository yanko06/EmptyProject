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


/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerialPort;

static void TaskBlinkRedLED(void *pvParameters); // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
static void TaskBlinkBlueLED(void *pvParameters); // Main Arduino Mega 2560, Freetronics EtherMega (Blue) LED Blink
static void TaskBlinkGreenLED(void *pvParameters); // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));

int main(void)
{

    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...

    xTaskCreate(
		TaskBlinkRedLED
		,  (const portCHAR *)"RedLED" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
		,  256				// Tested 9 free @ 208
		,  NULL
		,  3
		,  NULL ); // */

    xTaskCreate(
    		TaskBlinkBlueLED
    		,  (const portCHAR *)"BlueLED" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
    		,  256				// Tested 9 free @ 208
    		,  NULL
    		,  3
    		,  NULL ); // */

    xTaskCreate(
    		TaskBlinkGreenLED
    		,  (const portCHAR *)"GreenLED" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
    		,  256				// Tested 9 free @ 208
    		,  NULL
    		,  3
    		,  NULL ); // */

	avrSerialPrintf_P(PSTR("\r\n\nFree Heap Size: %u\r\n"),xPortGetFreeHeapSize() ); // needs heap_1 or heap_2 for this function to succeed.

	vTaskStartScheduler();

	avrSerialPrint_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...

}

/*-----------------------------------------------------------*/


static void TaskBlinkRedLED(void *pvParameters) // Main Red LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	DDRH = _BV(DDH3);

    while(1)
    {
    	PORTH &= ~_BV(PORTH3);       // Red LED off.
		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTH |=  _BV(PORTH3);       // Red LED on.
		vTaskDelayUntil( &xLastWakeTime, ( 1750 / portTICK_PERIOD_MS ) );

		//xSerialPrintf_P(PSTR("RedLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
    }
}

static void TaskBlinkBlueLED(void *pvParameters) // Main Blue LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	//DDRE = _BV(DDE5);

    while(1)
    {
    	vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_PERIOD_MS ) );
    	DDRE = _BV(DDE5);
    	PORTE &= ~_BV(PORTE5);      // Blue LED off.

		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTE |=  _BV(PORTE5);       // Blue LED on.
		vTaskDelayUntil( &xLastWakeTime, ( 1250 / portTICK_PERIOD_MS ) );

		//xSerialPrintf_P(PSTR("RedLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
    }
}

static void TaskBlinkGreenLED(void *pvParameters) // Main Green LED Flash
{
    (void) pvParameters;;
    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	//DDRE = _BV(DDE3);

    while(1)
    {
    	vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );
    	DDRE = _BV(DDE3);
    	PORTE &= ~_BV(PORTE3);       // Green LED off.

		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

		PORTE |=  _BV(PORTE3);       // Green LED on.
		vTaskDelayUntil( &xLastWakeTime, ( 750 / portTICK_PERIOD_MS ) );

		//xSerialPrintf_P(PSTR("RedLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
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

