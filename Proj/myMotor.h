#ifndef MOTOR_H
#define MOTOR_H
#include "MKL25Z4.h"                    // Device header
#include <math.h>
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "myInit.h"

#define MAX_DUTY_CYCLE 0x1D4C //7500 (50Hz)
#define TURN_DELAY 500 //milliseconds
#define MOVE_DELAY 500 //milliseconds
#define STEPS 20 //No.of direction moves

typedef enum state {
	FORWARD,
	REVERSE,
	RIGHT,
	LEFT,
	FWLEFT,
	RVLEFT,
	FWRIGHT,
	RVRIGHT,
	STOP,
	AUTO,
} state_t;

void stopMotors(void);
void reverse(state_t);
void forward(state_t);
void right(state_t);
void left(state_t);
void leftforward(state_t);
void leftreverse(state_t);
void rightforward(state_t);
void rightreverse(state_t);
void autoFwd(state_t s, int distance,int step);
void autoFwdAlt(state_t s, int step);
void reset(state_t s);
void autoLeft(state_t s,int step);
void autoRight(state_t s,int step);
void autoLeftFwd(state_t s,int step);
void autoRightFwd(state_t s,int step);
void challengeTwo(state_t s, int distance);




#endif
