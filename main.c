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
#include "wheels.h"

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
uint8_t statsBuffer[40*4];
int average;
char *tempPTR;
char temperature;
uint8_t temperatureTable[9];
/* Main program loop */
int main(void) __attribute__((OS_main));
typedef void (*TASK_POINTER)(void);
double * value;
double leftTemp=0;
double rightTemp=0;
void calculateAverageTmeperature(){
	for(int i = 1; i<5; i++){
		leftTemp += temperatureTable[i];
		rightTemp += temperatureTable[i+4];
	}
	leftTemp = leftTemp/4;
	rightTemp = rightTemp/4;
	char temperatureLeft = (char) (leftTemp);
	char temperatureRight = (char) (rightTemp);
	char temperatureAmbiant = (char) (temperatureTable[0]);
	//char concatenatedTemps = temperatureLeft;
	displayTopLine(&temperatureAmbiant, &temperatureRight);
	//changeLine();
	//displayBottomLine(&temperatureAmbiant, &temperatureRight, &temperatureLeft);
}
TASK_POINTER table[] =
{
	//rotateCentralServoMotor,
	//turnAroundAntiClockwise,
	//turnAroundClockwise,
	moveFoward
	//TaskStartIFS,
	//calculateAverageTmeperature
	//moveBackward,
	//CalculateTemperatureAverage,
	//ActivateLED,
	//DisplayTemp

};
void StartSchduler(void *pvParameters)
{
	start_Engine();
	vTaskDelay(( 2000 / portTICK_PERIOD_MS ));
	while(1)
	{
		int x;
		for ( x = 0; x < 1; x++ )
		{
			table[x]();
			vTaskDelay(( 2000 / portTICK_PERIOD_MS ));

	}

	}
}
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
	xTaskCreate(
				printSpeed
				,  (const portCHAR *)"Speed Monitor."
				,  256
				,  NULL
				,  3
				,  NULL );
	/*xTaskCreate(
			rotateCentralServoMotor
					,  (const portCHAR *)"Head Turn wut."
					,  256
					,  NULL
					,  3
					,  NULL );*/




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

