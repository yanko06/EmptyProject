/*
 * display.h
 *
 *  Created on: Feb 26, 2015
 *      Author: R3b1rth
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


void initLCD(void);
void display(char *str);
void clearDisplay();
void DisplayTemp(void *pvParameters);


#endif /* DISPLAY_H_ */
