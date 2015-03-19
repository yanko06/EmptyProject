#include "motion.h"
#include "FreeRTOS.h"
#include "serial.h"
/*
* MOTION_WHEEL_LEFT
* MOTION_WHEEL_RIGHT
* MOTION_SERVO_CENTER
 */

void start_Engine(void){
	motion_init();
}
void turnAroundAntiClockwise(void){
	avrSerialPrint_P(PSTR("\r\n\n\nturnAroundAntiClockwise\r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, 1100);
	motion_servo_start(MOTION_WHEEL_LEFT);
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, 1100);
	motion_servo_start(MOTION_WHEEL_RIGHT);
}
void turnAroundClockwise(void){
	avrSerialPrint_P(PSTR("\r\n\n\nturnAroundClockwise\r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, 4800);
	motion_servo_start(MOTION_WHEEL_LEFT);
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, 4800);
	motion_servo_start(MOTION_WHEEL_RIGHT);
}
void moveFoward(){
	avrSerialPrint_P(PSTR("\r\n\n\nmoveFoward\r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, 4800);
	motion_servo_start(MOTION_WHEEL_LEFT);
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, 1100);
	motion_servo_start(MOTION_WHEEL_RIGHT);
}
void moveBackward(){
	avrSerialPrint_P(PSTR("\r\n\n\nmoveBackward\r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, 1100);
	motion_servo_start(MOTION_WHEEL_LEFT);
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, 4800);
	motion_servo_start(MOTION_WHEEL_RIGHT);
}
void turnDegree(int degree){

}
/*double getSpeed(double * returned){
	avrSerialPrint_P(PSTR("\r\n\n\ngetSpeed\r\n"));
	int leftSpeed;
	uint32_t * leftTicCount;
	int rightSpeed;
	uint32_t * rightTicCount;
	uint32_t value = 0;
	leftSpeed = motion_enc_read(MOTION_WHEEL_LEFT, &leftTicCount);
	rightSpeed = motion_enc_read(MOTION_WHEEL_RIGHT, &rightTicCount);
	if(leftSpeed == 1){
		value = leftTicCount;
	}
	returned = 0.55 * value;
	avrSerialPrint_P(PSTR("\r\n\n\nSpeed: %d\r\n"),returned);
	return 0.55 * value;
}*/
void printSpeed(){
	avrSerialPrint_P(PSTR("\r\n\n\nprintSpeed\r\n"));
	int leftSpeed;
	uint32_t * leftTicCount;
	int rightSpeed;
	uint32_t * rightTicCount;
	uint32_t value = 0;
	leftSpeed = motion_enc_read(MOTION_WHEEL_LEFT, &leftTicCount);
	rightSpeed = motion_enc_read(MOTION_WHEEL_RIGHT, &rightTicCount);
	if(leftSpeed == 1){
		value = leftTicCount;
	}
	avrSerialPrintf_P(PSTR("Speed: %d"), value);
}
// L/R 1100/4800 Recle
// SERVO 1100 -> gauche, 4800 -> Droite
