/*----------------------------------------------------------------------------
 * LED CONTROL
 *---------------------------------------------------------------------------*/
 #include "myLED.h"
 
 uint8_t leds[8] = {LED_G2, LED_G3, LED_G4, LED_G5, LED_G8, LED_G9, LED_G10, LED_G11};
 
 void offLED(void) {
		PTD->PCOR |= MASK(LED_R1);
}

void greenFlashTwice() {
	int i;
	for(i = 0; i<2; i++) {
		PTC -> PSOR |= MASK(LED_G2) | MASK(LED_G3) | MASK(LED_G4) | MASK(LED_G5);
		PTA->PSOR |= (MASK(LED_G8) | MASK(LED_G9) | MASK(LED_G10));
		PTD->PSOR |= MASK(LED_G11);
		osDelay(GREEN_FLASH_TIME);
		PTC -> PCOR |= MASK(LED_G2) | MASK(LED_G3) | MASK(LED_G4) | MASK(LED_G5);
		PTA->PCOR |= (MASK(LED_G8) | MASK(LED_G9) | MASK(LED_G10));
		PTD->PCOR |= MASK(LED_G11);
		osDelay(GREEN_FLASH_TIME);
	}
}

void redStoppedState() {
	PTD -> PSOR |= MASK(LED_R1);
	osDelay(RED_STOP_TIME);
	PTD -> PCOR |= MASK(LED_R1);
	osDelay(RED_STOP_TIME);
}

void redMovingState() {
	PTD -> PSOR |= MASK(LED_R1);
	osDelay(RED_MOVING_TIME);
	PTD -> PCOR |= MASK(LED_R1);
	osDelay(RED_MOVING_TIME);
}

void greenStoppedState() {
		PTC -> PSOR |= MASK(LED_G2) | MASK(LED_G3) | MASK(LED_G4) | MASK(LED_G5);
		PTA->PSOR |= (MASK(LED_G8) | MASK(LED_G9) | MASK(LED_G10));
		PTD->PSOR |= MASK(LED_G11);
}

void greenMovingState(int c) {
	if (c < 4) {
				PTC -> PSOR |= MASK(leds[c]);
				osDelay(GREEN_MOVING_TIME);
				PTC -> PCOR |= MASK(leds[c]);
			} else if (c >= 4) {
				PTA -> PSOR |= MASK(leds[c]);
				PTD->PSOR |= MASK(leds[c]);
				osDelay(GREEN_MOVING_TIME);
				PTA -> PCOR |= MASK(leds[c]);
				PTD -> PCOR |= MASK(leds[c]);
			} 
}
