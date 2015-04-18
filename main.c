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

int leftTemp=0;
int rightTemp=0;
int speed = 0;
int distance = 0;
int STATE_STOP = -1;
int STATE_FOWARD = 0;
int STATE_BACKWARD = 1;
int STATE_TURN = 2;
int TURN_LEFT = 0;
int TURN_RIGHT = 1;
int TURN_NONE = -1;
static int direction_state = -1;
static int robot_turn = -1;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    temperatureTable[9];
/* Main program loop */
int main(void) __attribute__((OS_main));
double * value;


void PathManager(void *pvParameters){
	while(1){
		avrSerialPrintf_P(PSTR("\r\n PATH MANAGER \r\n"));
		initFoward();
		direction_state = STATE_FOWARD;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initBackward();
		direction_state = STATE_BACKWARD;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initFoward();
		direction_state = STATE_FOWARD;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initTurnLeft();
		direction_state = STATE_TURN;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initTurnRight();
		direction_state = STATE_TURN;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initFoward();
		direction_state = STATE_FOWARD;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
		initBackward();
		direction_state = STATE_BACKWARD;
		vTaskDelay(( 3000 / portTICK_PERIOD_MS ));
	}
}
void LEDManager(void *pvParameters){
	while(1){
		avrSerialPrintf_P(PSTR("\r\n -- LED MANAGER \r\n"));
		if(direction_state == STATE_FOWARD){
			turnOnLED(VERT);
		}else if(direction_state == STATE_BACKWARD){
			turnOnLED(ROUGE);
			vTaskDelay(( 500 / portTICK_PERIOD_MS ));
			turnOffLED(ROUGE);
		}else if(direction_state == STATE_TURN){
			turnOnLED(BLEU);
		}else{
			turnOffAllLED();
		}
		vTaskDelay(( 500 / portTICK_PERIOD_MS ));
	}
}
void WheelManager(void *pvParameters){
	while(1){
		avrSerialPrintf_P(PSTR("\r\n -- WHEEL MANAGER \r\n"));
		if(direction_state == STATE_FOWARD){
			initFoward();
		}else if(direction_state == STATE_BACKWARD){
			initBackward();
		}else if(direction_state == STATE_TURN){
			if(robot_turn == TURN_LEFT){
				initTurnLeft();
			}else{
				initTurnRight();
			}
		}
		vTaskDelay(( 200 / portTICK_PERIOD_MS ));
	}
}
void DisplayManager(void *pvParameters){
	char buffer[32];
	int n;
	while(1){
		avrSerialPrintf_P(PSTR("\r\n DISPLAY MANAGER \r\n"));
		while(1){
			calculateAverageTemperature();
			avrSerialPrintf_P(PSTR("\r\n V: %d D: %d        A:%d G:%d D:%d \r\n"), speed, distance, temperatureTable[0], leftTemp, rightTemp);
			n = sprintf(buffer,"V: %d D: %d        A:%d G:%d D:%d", speed, distance, temperatureTable[0], leftTemp, rightTemp);
			display(&buffer);
			vTaskDelay(( 20 / portTICK_PERIOD_MS ));
		}
	}
}
void ThermicManager(void *pvParameters){
	while(1){
		avrSerialPrintf_P(PSTR("\r\n -- THERMIC MANAGER \r\n"));
		TaskStartIFS();
		rotateCentralServoLeft();
		vTaskDelay(( 200 / portTICK_PERIOD_MS ));
		rotateCentralServoRight();
	}
}
void DSCalculatorManager(void *pvParameters){
	while(1){
		avrSerialPrintf_P(PSTR("\r\n -- CALCULATOR MANAGER \r\n"));
		vTaskDelay(( 300 / portTICK_PERIOD_MS ));
	}
}


int main(void)
{
    // turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal( USART0, 115200, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	avrSerialPrint_P(PSTR("\r\n\n\nRobot powering UP!!\r\n"));
	taskDISABLE_INTERRUPTS();
	I2C_Master_Initialise(0XD4);
	taskENABLE_INTERRUPTS();
	xTaskCreate(
			PathManager
			,  (const portCHAR *)"Path Scheduler"
			,  256
			,  NULL
			,  3
			,  NULL );
	xTaskCreate(
			LEDManager
			,  (const portCHAR *)"Turn Servo Head."
			,  256
			,  NULL
			,  1
			,  NULL );
	xTaskCreate(
			WheelManager
			,  (const portCHAR *)"Update Display."
			,  256
			,  NULL
			,  4
			,  NULL );
	xTaskCreate(
			DisplayManager
			,  (const portCHAR *)"Speed Regulator."
			,  256
			,  NULL
			,  2
			,  NULL );
	xTaskCreate(
			ThermicManager
			,  (const portCHAR *)"Turn On LED."
			,  256
			,  NULL
			,  2
			,  NULL );
	xTaskCreate(
			DSCalculatorManager
			,  (const portCHAR *)"Distance and speed."
			,  256
			,  NULL
			,  2
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

/*------------------HELPER-----------------------*/

void calculateAverageTemperature(){
	for(int i = 1; i<5; i++){
		leftTemp += temperatureTable[i];
		rightTemp += temperatureTable[i+4];
	}
	leftTemp = leftTemp/4;
	rightTemp = rightTemp/4;
}
