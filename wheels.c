#include "motion.h"
#include "FreeRTOS.h"
#include "serial.h"
/*
* MOTION_WHEEL_LEFT
* MOTION_WHEEL_RIGHT
* MOTION_SERVO_CENTER
 */
#define LEFT		                0
#define LEFT_ENCODER                1
#define RIGHT          			    1
#define RIGHT_ENCODER  			    0
#define REF_SPEED	            (int) 55000
#define INIT_LEFT_FOWARD				2750 // 2550 //2615 //2750
#define INIT_LEFT_BACKWARD				2550
#define INIT_RIGHT_FOWARD				3250 //3250 //3350 stop //3450
#define INIT_RIGHT_BACKWARD				3450
//#define Kp            			    (uint32_t) 1/1000

static int currentLeftSpeed;
static int currentRightSpeed;
static int oldLeftError = 0;
static int oldRightError = 0;

static int distanceWheels = 0;

void start_Engine(void){
	motion_init();
}

void initFoward(){
	avrSerialPrintf_P(PSTR("\r\n -- FOWARD \r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, INIT_LEFT_FOWARD);
	motion_servo_start(MOTION_WHEEL_LEFT);
	currentLeftSpeed = INIT_LEFT_FOWARD;
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, INIT_RIGHT_FOWARD);
	motion_servo_start(MOTION_WHEEL_RIGHT);
	currentRightSpeed = INIT_RIGHT_FOWARD;
}
void initBackward(){
	avrSerialPrintf_P(PSTR("\r\n -- BACKWARD \r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, INIT_LEFT_BACKWARD);
	motion_servo_start(MOTION_WHEEL_LEFT);
	currentLeftSpeed = INIT_LEFT_FOWARD;
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, INIT_RIGHT_BACKWARD);
	motion_servo_start(MOTION_WHEEL_RIGHT);
	currentRightSpeed = INIT_RIGHT_FOWARD;
}
void initTurnLeft(){
	avrSerialPrintf_P(PSTR("\r\n -- TURN LEFT \r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, INIT_LEFT_BACKWARD);
	motion_servo_start(MOTION_WHEEL_LEFT);
	currentLeftSpeed = INIT_LEFT_FOWARD;
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, INIT_RIGHT_FOWARD);
	motion_servo_start(MOTION_WHEEL_RIGHT);
	currentRightSpeed = INIT_RIGHT_FOWARD;
}
void initTurnRight(){
	avrSerialPrintf_P(PSTR("\r\n -- TURN RIGHT \r\n"));
	motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, INIT_LEFT_FOWARD);
	motion_servo_start(MOTION_WHEEL_LEFT);
	currentLeftSpeed = INIT_LEFT_FOWARD;
	motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, INIT_RIGHT_BACKWARD);
	motion_servo_start(MOTION_WHEEL_RIGHT);
	currentRightSpeed = INIT_RIGHT_FOWARD;
}

void setSpeed(int Side, int Speed){
	if(Side==LEFT){
		//avrSerialPrintf_P(PSTR("\r\n\n\nSet speed: LEFT \r\n"));
		currentLeftSpeed = Speed;
		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, Speed);
	}else{
		//avrSerialPrintf_P(PSTR("\r\n\n\nSet speed: RIGHT \r\n"));
		currentRightSpeed = Speed;
		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, Speed);
	}
}
int getSpeed(int Side){
	return (currentLeftSpeed + currentRightSpeed) / 2 * 0.55/100;
}
void validateSpeed(){
	while(1)
	{
	//avrSerialPrint_P(PSTR("\r\n\n\nprintSpeed\r\n"));
	uint32_t leftSpeed = 0;
	uint32_t leftTicCount;
	uint32_t rightSpeed = 0;
	uint32_t rightTicCount;
	uint32_t value = 0;
	uint32_t value2  = 0;
	leftSpeed = motion_enc_read(LEFT_ENCODER, &leftTicCount);
	rightSpeed = motion_enc_read(RIGHT_ENCODER, &rightTicCount);
	if(leftSpeed == 1){
		value = leftTicCount;
		calculateNewSpeed(LEFT, value);
	}
	if(rightSpeed == 1)
	{
		value2 = rightTicCount;
		calculateNewSpeed(RIGHT, value2);
	}
	//avrSerialPrintf_P(PSTR("Speed Left: %u \r\n"), value);
	//avrSerialPrintf_P(PSTR("Speed Right: %u \r\n"), value2);
	vTaskDelay(( 200 / portTICK_PERIOD_MS ));
	}
}

void calculateNewSpeed(int Side, uint32_t latestTickCount){
	uint32_t Ut;
	uint32_t Ut1;
	uint32_t Ek;
	uint32_t Ek1;
	if(Side==LEFT){
		//avrSerialPrintf_P(PSTR("Calculate LEFT:"));
		//avrSerialPrintf_P(PSTR("Tick: %u"), latestTickCount);
		Ek = 55000 - latestTickCount;
		Ut1 = currentLeftSpeed;
		Ek1 = oldLeftError;
	}else{
		//avrSerialPrintf_P(PSTR("Calculate RIGHT:"));
		//avrSerialPrintf_P(PSTR("Tick: %u"), latestTickCount);
		Ek = 55000 - latestTickCount;
		Ut1 = currentRightSpeed;
		Ek1 = oldRightError;
	}
	Ut = Ut1 + (Ek)/1000;
	//avrSerialPrintf_P(PSTR(":Ut: %u"), Ut);
	//avrSerialPrintf_P(PSTR(":Ut1: %u"), Ut1);
	//avrSerialPrintf_P(PSTR(":Ek: %d \r\n "), Ek);
	//avrSerialPrintf_P(PSTR("Ek1: %d -- "), Ek1);
	//avrSerialPrintf_P(PSTR("\r\n -- -- -- -- -- -- -- -- \r\n"));
	if(Side==LEFT){
		oldLeftError = Ek;
	}else{
		oldRightError = Ek;
	}
	//setSpeed(Side,Ut);
}
void rotateCentralServoLeft(){
	motion_servo_set_pulse_width(MOTION_SERVO_CENTER, 1100);
	motion_servo_start(MOTION_SERVO_CENTER);
	vTaskDelay(( 500 / portTICK_PERIOD_MS ));
	motion_servo_stop(MOTION_SERVO_CENTER);
}
void rotateCentralServoRight(){
	motion_servo_set_pulse_width(MOTION_SERVO_CENTER, 4800);
	motion_servo_start(MOTION_SERVO_CENTER);
	vTaskDelay(( 500 / portTICK_PERIOD_MS ));
	motion_servo_stop(MOTION_SERVO_CENTER);
}

int getDistance(){
	return distanceWheels;
}
// L/R 1100/4800 Recle
// SERVO 1100 -> gauche, 4800 -> Droite
