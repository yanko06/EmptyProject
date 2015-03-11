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
#include "display.h"

/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerial1Port;
extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/
int average;
char *temp2;
char temp;
uint8_t TArray[9];
/* Main program loop */
int main(void) __attribute__((OS_main));
typedef void (*TASK_POINTER)(void);
#define TABLE_SIZE


void CalculateAverage(void *pvParameters)
{

	average = 0;
		for (int i = 1; i < 9; i++){
			average = average + TArray[i];
		}
		average = average/8;

		temp = (char)(average);
		temp2 = &temp;
}
void ActivateLED(void *pvParameters)
{

	if ((int) average < 20){
				turnOffLED(0);
				turnOffLED(2);
				turnOnLED(1);
		}
		else if (average < 30){
				turnOffLED(0);
				turnOffLED(1);
				turnOnLED(2);
		}
		else{
				turnOffLED(1);
				turnOffLED(2);
				turnOnLED(0);
		}
}
void DisplayTemp(void *pvParameters)
{
	display(temp2);
}
TASK_POINTER table[] =
{
	TaskStartIFS,
	CalculateAverage,
	ActivateLED,
	DisplayTemp

};
void StartSchduler(void *pvParameters)
{
	while(1)
	{
		int x;
		for ( x = 0; x < 4; x++ )
		{
			table[x]();

	}
		vTaskDelay(( 250 / portTICK_PERIOD_MS ));
	}
}
/*void TaskStartIFSTest(void *pvParameters) // Main Red LED Flash
{
	TickType_t xLastWakeTime;
	/* Variable used in vTaskDelayUntil. Needs to be set once to allow for a valid start
		time for the vTaskDelayUntil. After this point, vTaskDelayUntil handles itself.
		xLastWakeTime = xTaskGetTickCount();

	//int average;
	//char temp;
	//char *temp2;

	while (1){
	TaskStartIFS(&TArray[0]);

	vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_PERIOD_MS ) );

	/*for (int k = 0; k < 9; k++){
			avrSerialPrintf_P(PSTR("%d "), TArray[k]);
	}
	avrSerialPrintf_P(PSTR("\n %d "), average);

	avrSerialPrintf_P(PSTR("\r Temp: %d \n"), *temp2);
	display(temp2);

	if ((int) average < 20){
			turnOffLED(0);
			turnOffLED(2);
			turnOnLED(1);
	}
	else if (average < 30){
			turnOffLED(0);
			turnOffLED(1);
			turnOnLED(2);
	}
	else{
			turnOffLED(1);
			turnOffLED(2);
			turnOnLED(0);
	}
	}
}*/

int main(void)
{
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n"));
	taskDISABLE_INTERRUPTS();
	I2C_Master_Initialise(0XD4);
	taskENABLE_INTERRUPTS();
	xTaskCreate(
			StartSchduler
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

