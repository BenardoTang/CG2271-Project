#ifndef INIT_H
#define INIT_H
#include "MKL25Z4.h"                    // Device header
#include <math.h>

#define MASK(x) (1 << (x))

// Defining Motors
#define LEFT_BK 0 // PTB0 TPM1_CH0
#define LEFT_FW 1 // PTB1 TPM1_CH1
#define RIGHT_BK 2 // PTB2 TPM2_CH0
#define RIGHT_FW 3 // PTB3 TPM2_CH1
#define UART_RX_PORTE23 23 //PTE 23 RX
#define BAUD_RATE 9600
#define UART2_INT_PRIO 128	

// Rear LED Pin Config
#define LED_R1 2 // PortD Pin 2
#define LED_G2 12 // PortC Pin 12
#define LED_G3 13 // PortC Pin 13
#define LED_G4 16 // PortC Pin 16
#define LED_G5 17 // PortC Pin 17
#define LED_G8 16 // PortA Pin 16
#define LED_G9 17 // PortA Pin 17
#define LED_G10 13 // port a 13
#define LED_G11 7 // PortD Pin 7


// PWM
#define STOP_MOTOR 11
#define MASK(x) (1 << (x))
#define FW_MASK 0x08
#define RV_MASK 0x02
#define STOP_MASK 0x0B // 11
#define RT_MASK 0x06
#define LT_MASK 0x04
#define FWLT_MASK 0x07
#define FWRT_MASK 0x09
#define RVLT_MASK 0x01
#define RVRT_MASK 0x03
#define RUN_CHALLENGE_TWO 12
#define MID_SPEED 13
#define DETECT_DISTANCE 14
#define AUTO_FWD 15
#define RESET_FWD 16



// SOUND
#define PTD0_Pin 0
//#define PTD1_Pin 1

//SENSOR
#define TRIG_PIN 3 // PortD Pin 3
#define ECHO_PIN 6 // PortD Pin 6

// MISCELLANEOUS MACROS

//#define CONNECT 0x01
#define CONNECT 69 
#define THE_END 0xFF
#define END_TUNE 253
#define CLEAR_IRQ 0xffffffff
#define INIT_VAR 0xFE
#define RESET 0x00


void initClockGate(void);
void initPWM(void);
void initUART2(uint32_t);
void initLED(void);
void initAudio(void);
void initSensor(void);

#endif

