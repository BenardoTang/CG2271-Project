/*----------------------------------------------------------------------------
 * INITIALISATION FUNCTIONS
 *---------------------------------------------------------------------------*/
#include "myInit.h"

/* INIT CLOCK GATING */
void initClockGate() {
	// Enable Clock Gating for PORTB and PORTE
	SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTE_MASK) | (SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTD_MASK) | (SIM_SCGC5_PORTC_MASK);
	// Enable Clock Gating for UART2
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
		//Enable clock gating for Timer0, Timer 1 and Timer 2
	SIM->SCGC6 = (SIM_SCGC6_TPM0_MASK) | (SIM_SCGC6_TPM1_MASK) | (SIM_SCGC6_TPM2_MASK);
}


/* INITIALISE PWM PINS YAY*/
void initPWM() {
	// Configure Mode 3 for PWM pin operation
	PORTB->PCR[LEFT_FW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LEFT_FW] |= PORT_PCR_MUX(3);
	PORTB->PCR[LEFT_BK] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LEFT_BK] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[RIGHT_FW] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RIGHT_FW] |= PORT_PCR_MUX(3);
	PORTB->PCR[RIGHT_BK] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RIGHT_BK] |= PORT_PCR_MUX(3);
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK OR MCGPLLCLK/2
	
	//set modulo value 48000000/128 = 375000, 375000Hz/50Hz = 7500	
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	//Edge-Aligned PWM
	//CMOD - 1 and PS - 111 (128)
	TPM1_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
	TPM1_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)
  
	TPM2_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
	TPM2_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)
	
	//enable PWM on TPM1 channel 0 - PTB0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//enable PWM on TPM1 channel 1 - PTB1
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//enable PWM on TPM2 channel 0 - PTB2
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//enable PWM on TPM2 channel 1 - PTB3
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

/*	Init UART2	*/
void initUART2(uint32_t baud_rate){
	
	uint32_t divisor, bus_clock;
	
	PORTE-> PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE-> PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	bus_clock = DEFAULT_SYSTEM_CLOCK/2;
	divisor = bus_clock / (baud_rate*16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	UART2->C2 |= (UART_C2_RE_MASK);
	NVIC_SetPriority(UART2_IRQn, 128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	UART2->C2 |= UART_C2_RIE_MASK;

}

/* LED GPIO Initialization Function */
void initLED(void) {

 // Configure MUX settings for rear led
 PORTD->PCR[LED_R1] &= ~PORT_PCR_MUX_MASK;
 PORTD->PCR[LED_R1] |= PORT_PCR_MUX(1);
 
 // Configure MUX settings for front led
 PORTC->PCR[LED_G2] &= ~PORT_PCR_MUX_MASK;
 PORTC->PCR[LED_G2] |= PORT_PCR_MUX(1);
 PORTC->PCR[LED_G3] &= ~PORT_PCR_MUX_MASK;
 PORTC->PCR[LED_G3] |= PORT_PCR_MUX(1);
 PORTC->PCR[LED_G4] &= ~PORT_PCR_MUX_MASK;
 PORTC->PCR[LED_G4] |= PORT_PCR_MUX(1);
 PORTC->PCR[LED_G5] &= ~PORT_PCR_MUX_MASK;
 PORTC->PCR[LED_G5] |= PORT_PCR_MUX(1);
 
 PORTA->PCR[LED_G8] &= ~PORT_PCR_MUX_MASK;
 PORTA->PCR[LED_G8] |= PORT_PCR_MUX(1);
 PORTA->PCR[LED_G9] &= ~PORT_PCR_MUX_MASK;
 PORTA->PCR[LED_G9] |= PORT_PCR_MUX(1);
 
 PORTA->PCR[LED_G10] &= ~PORT_PCR_MUX_MASK;
 PORTA->PCR[LED_G10] |= PORT_PCR_MUX(1);
 PORTD->PCR[LED_G11] &= ~PORT_PCR_MUX_MASK;
 PORTD->PCR[LED_G11] |= PORT_PCR_MUX(1);

 // Set Data Direction Registers for PortB and PortD
 PTD->PDDR |= MASK(LED_R1);
 PTC->PDDR |= (MASK(LED_G2) | MASK(LED_G3) | MASK(LED_G4) | MASK(LED_G5));
 PTA->PDDR |= (MASK(LED_G8) | MASK(LED_G9) | MASK(LED_G10));
 PTD->PDDR |= MASK(LED_G11);


}

void initAudio() {

	// Configure Mode 3 for PWM pin operation
	PORTD->PCR[PTD0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD0_Pin] |= PORT_PCR_MUX(4);
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK OR MCGPLLCLK/2
	
	//set modulo value 48000000/128 = 375000, 375000Hz/50Hz = 7500	
	TPM0->MOD = 7500;
	
	//Edge-Aligned PWM
	//CMOD - 1 and PS - 111 (128)
	TPM0_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
	TPM0_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)

	//enable PWM on TPM0 channel 0 - PTD1
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
}


// sr-04 SENSOR GPIO Initialization Function 
void initSensor(void) {

 // Configure MUX settings for sensor
 PORTD->PCR[TRIG_PIN] &= ~PORT_PCR_MUX_MASK;
 PORTD->PCR[TRIG_PIN] |= PORT_PCR_MUX(4);
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK OR MCGPLLCLK/2
	
	//set modulo value 48000000/128 = 375000, 375000Hz/50Hz = 7500
	TPM0->MOD = 7500;
	
	//Edge-Aligned PWM
	//CMOD - 1 and PS - 111 (128)
	TPM0_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
	TPM0_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)

	//enable PWM on TPM0 channel 3 - PTD3
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
 
 // Configure MUX settings for ECHO
 PORTD->PCR[ECHO_PIN] &= ~PORT_PCR_MUX_MASK;
 PORTD->PCR[ECHO_PIN] |= PORT_PCR_MUX(1);

 // Set Data Direction Registers for TRIG_PIN(OUTPUT) , ECHO_PIN(INPUT)
 PTD->PDDR &= ~MASK(ECHO_PIN); //INPUT
}

