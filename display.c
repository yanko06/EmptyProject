/*
 * display.c
 *
 *  Created on: Feb 26, 2015
 *      Author: R3b1rth
 */

#include "FreeRTOS.h"
#include "task.h"
#include "serial.h"

extern xComPortHandle xSerial1Port;
xComPortHandlePtr xSerialPortPtr = &xSerial1Port;

void initLCD(void){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
};
void display(char *str){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	avrSerialPrintf_P(PSTR("\r\n\n\nDisplay String: %s\r\n"), str);
	uint8_t * cvn = (uint8_t *) str;
	xSerialFlush(xSerialPortPtr);
	xSerialxPrint(xSerialPortPtr, cvn);
};
void clearDisplay(){
	char * cptr = "124 1";
	uint8_t * cvn = (uint8_t *) cptr;
	xSerialFlush(xSerialPortPtr);
	xSerialxPrint(xSerialPortPtr, cvn);
};
