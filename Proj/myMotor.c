/*----------------------------------------------------------------------------
 * MOTOR CONTROL
 *---------------------------------------------------------------------------*/
#include "myMotor.h"

volatile double slow = 2; //Used for turning speed
volatile int stop = 0; //Used for turning speed
volatile int j[STEPS] = {0};
volatile int i = 0;
volatile int k[STEPS] = {0};


/** Stop the motors**/
void stopMotors(){
	TPM1->MOD = 0;
	TPM1_C0V = 0; // stop left fw
	TPM1_C1V = 0; // stop left bk
	
	TPM2->MOD = 0;
	TPM2_C0V = 0; // stop right fw
	TPM2_C1V = 0; // stop right bk
	
}

/** Move Forward **/ 
void forward(state_t s) {
	if(s != FORWARD) {
		TPM1_C1V = 0;
		TPM2_C1V = 0;
	}
  TPM1->MOD = 7500;
	TPM1_C0V = MAX_DUTY_CYCLE;
	TPM2->MOD = 7500;
	TPM2_C0V = MAX_DUTY_CYCLE;
}

/** Move Reverse **/
void reverse(state_t s) {
	 if(s != REVERSE) {
		TPM1_C0V = 0;
		TPM2_C0V = 0;
	 }
	 TPM1->MOD = 7500;
	 TPM1_C1V = MAX_DUTY_CYCLE;
	 TPM2->MOD = 7500;
	 TPM2_C1V = MAX_DUTY_CYCLE;
}

/** Rotate Right **/
void right(state_t s) {
	 if (s != RIGHT) {
		  TPM1_C1V = 0;
		  TPM2_C0V = 0;
	 }
	 TPM1->MOD = 7500*slow;
	 TPM1_C0V = MAX_DUTY_CYCLE;
	 TPM2->MOD = 7500*slow;
	 TPM2_C1V = MAX_DUTY_CYCLE;
}

/** Rotate Left **/
void left(state_t s) {
	 if (s != LEFT) { 
		 TPM1_C0V = 0;
		 TPM2_C1V = 0;
	 }
	 TPM1->MOD = 7500*slow;
	 TPM1_C1V = MAX_DUTY_CYCLE;
	 TPM2->MOD = 7500*slow;
	 TPM2_C0V = MAX_DUTY_CYCLE;
}

/** Curved Reverse Left**/
void leftreverse(state_t s) {
	if(s != RVLEFT) {
		TPM1_C0V = 0;
		TPM2_C0V = 0;
	}
	 TPM1->MOD = 7500*slow; 
	 TPM1_C1V = MAX_DUTY_CYCLE;
	 TPM2->MOD = 7500;
	 TPM2_C1V = MAX_DUTY_CYCLE;
	
}

/** Curved Forward RIGHT**/
void rightforward(state_t s) {
	if(s != FWRIGHT) {
		TPM1_C1V = 0;
		TPM2_C1V = 0;
	}
	 TPM2->MOD = 7500*(slow*2); //right
	 TPM2_C0V = MAX_DUTY_CYCLE;
	 TPM1->MOD = 7500/(slow*4); //left
	 TPM1_C0V = MAX_DUTY_CYCLE;
	
}

/** Curved Reverse Right**/
void rightreverse(state_t s) {
	if(s != RVRIGHT) {
		TPM1_C0V = 0;
		TPM2_C0V = 0;
	}
	 TPM2->MOD = 7500*slow; //right
	 TPM2_C1V = MAX_DUTY_CYCLE;
	 TPM1->MOD = 7500; //left
	 TPM1_C1V = MAX_DUTY_CYCLE;
	
}

/** Curved Forward LEF**/
void leftforward(state_t s) {
	if(s != FWLEFT) {
		TPM1_C1V = 0;
		TPM2_C1V = 0;
	}
	 TPM2->MOD = 7500/(slow*4); //right
	 TPM2_C0V = MAX_DUTY_CYCLE;
	 TPM1->MOD = 7500*slow*2; //left
	 TPM1_C0V = MAX_DUTY_CYCLE;
	
}
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void delay100x(volatile uint32_t nof) {
	for(int i =0;i<100;i++) {
		delay(nof);
	}
}

void reset(state_t s){
	i = 0;
	for(int q=0;q<STEPS;q++){
		j[q] =0;
		k[q] = 0;
	}
}
void autoFwd(state_t s, int distance,int step){
	int scaled_distance = distance*10000;
	while(k[step] < scaled_distance) // 
	{
		TPM1->MOD = 7500;
		TPM1_C0V = MAX_DUTY_CYCLE;
		TPM2->MOD = 7500;
		TPM2_C0V = MAX_DUTY_CYCLE;
		k[step]++;
	}	
		stopMotors();
		//sTILL NEED TO GO STRAIGHT BACK TO BOX		
}

void autoFwdAlt(state_t s, int step){
	while(j[step] < 110000) // 
	{
		TPM1->MOD = 7500;
		TPM1_C0V = MAX_DUTY_CYCLE;
		TPM2->MOD = 7500;
		TPM2_C0V = MAX_DUTY_CYCLE;
		j[step]++;
	}	
		stopMotors();
}

void autoLeft(state_t s,int step){
		while(j[step] < 7500) // turn left need to test
		{
		 TPM1->MOD = 7500*slow;
		 TPM1_C1V = MAX_DUTY_CYCLE;
		 TPM2->MOD = 7500*slow;
		 TPM2_C0V = MAX_DUTY_CYCLE;
			j[step]++;
		}
		stopMotors();
}


void autoRight(state_t s,int step){
		while(j[step] < 8000) // turn right need to test
		{
		 TPM1->MOD = 7500*slow;
		 TPM1_C0V = MAX_DUTY_CYCLE;
		 TPM2->MOD = 7500*slow;
		 TPM2_C1V = MAX_DUTY_CYCLE;
			j[step]++;
		}
		stopMotors();
}

void autoLeftFwd(state_t s,int step){
		while(j[step] < 3500) // curved left need to test
		{
		 TPM2->MOD = 7500/(slow*2); //right
		 TPM2_C0V = MAX_DUTY_CYCLE;
		 TPM1->MOD = 7500*slow; //left
		 TPM1_C0V = MAX_DUTY_CYCLE;
			j[step]++;
		}
		stopMotors();
}

void autoRightFwd(state_t s,int step){
		while(j[step] < 4000) // curved right need to test
		{
		 TPM1->MOD = 7500/(slow*2); //left
		 TPM1_C0V = MAX_DUTY_CYCLE;
		 TPM2->MOD = 7500*slow; //right
		 TPM2_C0V = MAX_DUTY_CYCLE;
			j[step]++;
		}
		stopMotors();
}

void challengeTwo(state_t s,int distance){
		int scaled_distance = distance*10000;
		
		autoFwd(AUTO,distance,0);
		autoLeft(AUTO,0);
		autoFwdAlt(AUTO,1);
		autoFwdAlt(AUTO,2);
		autoRight(AUTO,3);
		autoFwdAlt(AUTO,4);
		autoFwdAlt(AUTO,5);
		autoRight(AUTO,6);
		autoFwdAlt(AUTO,7);
		autoFwdAlt(AUTO,8);
		autoFwd(AUTO,6,1);
		autoRight(AUTO,9);
		autoFwdAlt(AUTO,10);
		autoFwdAlt(AUTO,11);
		autoFwdAlt(AUTO,17);
		autoFwd(AUTO,distance+5,2);

}
