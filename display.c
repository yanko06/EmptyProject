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

void clearDisplay(){
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0xFE);
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0x01);
};

void display(char *str){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	clearDisplay();

	avrSerialxPrintf_P(xSerialPortPtr,PSTR("T: %d"), *str);
	xSerialFlush(xSerialPortPtr);
	vSerialClose(xSerialPortPtr);
};

