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
extern char *tempPTR;
extern char temperature;
void initLCD(void){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
};

void clearDisplay(){
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0xFE);
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0x01);
};

void changeLine(){
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0xFE);
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0x80);
	xSerialxPrintf_P(xSerialPortPtr, PSTR("%c"), 0x40);
};

void displayBottomLine(char *ambiantTemp, char *averageRightTemp, char *averageLeftTemp){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	clearDisplay();
	avrSerialxPrintf_P(xSerialPortPtr,PSTR("A %d, G %d, D %d"), *ambiantTemp, *averageRightTemp, *averageLeftTemp);
	xSerialFlush(xSerialPortPtr);
	vSerialClose(xSerialPortPtr);
};

void displayTopLine(char *speed, char *distance){
	xSerial1Port = xSerialPortInitMinimal( USART1, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)
	clearDisplay();
	avrSerialxPrintf_P(xSerialPortPtr,PSTR("S %d, D %d"), *speed, *distance);
	xSerialFlush(xSerialPortPtr);
	vSerialClose(xSerialPortPtr);
};

void DisplayTemp(void *pvParameters)
{
	//display(tempPTR);
};

