#ifndef SENSOR_H
#define SENSOR_H
#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "myInit.h"
#include <math.h>
#include "myMotor.h"

// Add delay times
#define PULSE_DELAY 1 // 10us
#define SPEED_OF_SOUND 0.0343 // 343 METRES PER SEC


float checkDistance(void);
void startCount(void);
void runPulse(void);
void recalibrate(void);
float test(void);


#endif
