/*----------------------------------------------------------------------------
 * AUDIO CONTROL
 *---------------------------------------------------------------------------*/
#include "myAudio.h"
#include "myInit.h"

// change this to make the song slower or faster
int tempo = 140;

volatile uint8_t end, connect;

/**
* Notes of a melody followed by duration.
* A 4 means a quarter note, 8 an eighth, 16 sixteenth and so on.
* Negative numbers are used to represent dotted notes.
* So, a -4 means a dotted quarter note i.e. a quarter + eighteenth.
*/


// cool bgm from https://youtu.be/DQweLHdlVWo?t=36
int themeMelody[] = {

	NOTE_CS5,4, NOTE_FS5,8, NOTE_DS5,8, NOTE_E5,8, NOTE_GS5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_GS5,8, NOTE_B5,4,
	NOTE_E5,4, NOTE_B4,8, NOTE_FS5,8, NOTE_GS5,8, NOTE_AS5,8, NOTE_CS6,4,REST,4, NOTE_DS5,8, NOTE_CS6,8, NOTE_B4,8,
	NOTE_AS5,8, NOTE_B5,4, NOTE_FS5,4, NOTE_B5,8, NOTE_AS5,8, NOTE_GS5,8, NOTE_FS5,8, NOTE_GS5,4, NOTE_DS5,4, NOTE_E5,8,
	NOTE_FS5,8, NOTE_GS5,4, NOTE_GS5,8, NOTE_AS5,8, NOTE_B5,4, NOTE_CS6,8, NOTE_B5,8, NOTE_AS5,8, NOTE_B5,8, NOTE_CS6,4,
	NOTE_FS5,4, NOTE_B5,8, NOTE_CS6,8, NOTE_DS5,8, NOTE_E6,8, /* MIGHT HAVE REST HERE=REST,4,*/ NOTE_E6,8, NOTE_DS5,8, NOTE_CS6,8,
	NOTE_GS5,8, NOTE_AS5,8, NOTE_B5,8, NOTE_CS6,8, NOTE_DS6,8, NOTE_B5,8, NOTE_DS6,4, NOTE_GS5,8, NOTE_E5,8, NOTE_FS5,8,
	NOTE_GS5,8, NOTE_B5,4, NOTE_E5,8, NOTE_E5,8, NOTE_AS5,8, NOTE_FS5,8, NOTE_GS5,8, NOTE_AS5,8, NOTE_CS6,4, REST,8, NOTE_FS5,8,
	NOTE_DS6,8, NOTE_CS6,8, NOTE_B5,8, NOTE_AS5,8, NOTE_B5,4, REST,4, NOTE_B5,8, NOTE_AS5,8, NOTE_GS5,8, NOTE_FS5,8, NOTE_GS5,4,
	NOTE_DS5,4, NOTE_E5,8, NOTE_FS5,8, NOTE_GS5,4, NOTE_GS5,8, NOTE_AS5,4, NOTE_CS6,8, NOTE_B5,8, NOTE_AS5,8, NOTE_B5,8, NOTE_CS6,8,
	NOTE_FS5,8, NOTE_AS4,8, NOTE_CS6,8, NOTE_B5,4, NOTE_FS5,4, NOTE_B5,4, REST,4, NOTE_B4,4, NOTE_DS6,4, NOTE_B5,2
 
};

// Despacito opening sound, played at the start. adapted from https://www.youtube.com/watch?v=2Lok4QqFmWU
int despacito [] = {
	NOTE_D5, 2, NOTE_CS5, 2, NOTE_B4,4, NOTE_FS4,8, REST, 2
};

int endTune [] = { // Final fantasy victory fanfare https://www.youtube.com/watch?v=-YCN-a0NsNk&list=PLd2wUCjblM4EEY6jkfIquVZhljCtTb74O&index=2
	NOTE_D5,8, NOTE_D5,8, NOTE_D5,8, NOTE_D5,4, NOTE_AS, 4, NOTE_C5, 4, NOTE_D5,-8, NOTE_C5, 8, NOTE_D5,2
};



/** Duty Cycle Function **/

uint8_t dutyCycle(int period, int div) {
	return (period/div);
}

/** PLAY MUSIC FUNCTIONS **/
	// Sizeof gives the number of bytes, each int value is composed of two bytes (16 bits).
	// There are two values per note (pitch and duration), so for each note there are four bytes.


void playCoolTheme() {
	int notes = sizeof(themeMelody) / sizeof(themeMelody[0]);

	// This calculates the duration of a whole note in ms (60s/tempo)*4 beats.
	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	uint32_t period;


		for(int i = 0; (i<notes && end != THE_END && connect != CONNECT) ; i+=2) {
			// Calculates the duration of each note.
			divider = themeMelody[i + 1];
			if (divider > 0) {
				// Regular note, just proceed.
				noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
				// Dotted notes are represented with negative durations.
				noteDuration = (wholenote) / (int)fabs((float)divider);
				noteDuration *= 1.5; // Increases the duration in half for dotted notes.
			}
			period = TO_MOD(themeMelody[i]);
			TPM0->MOD = period;
			TPM0_C0V = dutyCycle(period, 4); //25% duty cycle
			osDelay((noteDuration)*5/9);
			TPM0->MOD = 0;
			TPM0_C0V = 0;
			osDelay((noteDuration)*25/81);
		}
}

void playStart () {
	int notes = sizeof(despacito) / sizeof(despacito[0]);

	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	uint32_t period;

		for(int i = 0; i<notes; i+=2) {
			divider = despacito[i + 1];
			if (divider > 0) {
				noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
				noteDuration = (wholenote) / (int)fabs((float)divider);
				noteDuration *= 1.5;
			}
			period = TO_MOD(despacito[i]);
			TPM0->MOD = period;
			TPM0_C0V = dutyCycle(period, 8); //12.5% duty cycle
			osDelay((noteDuration)*5/9);
			TPM0->MOD = 0;
			TPM0_C0V = 0;
			osDelay((noteDuration)*25/81);
		}
		connect = CHANGE_VAR;
}

void playEnd() {
	int notes = sizeof(endTune) / sizeof(endTune[0]);

	// This calculates the duration of a whole note in ms (60s/tempo)*4 beats.
	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	uint32_t period;


		for(int i = 0; (i<notes) ; i+=2) {
			// Calculates the duration of each note.
			divider = endTune[i + 1];
			if (divider > 0) {
				// Regular note, just proceed.
				noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
				// Dotted notes are represented with negative durations.
				noteDuration = (wholenote) / (int)fabs((float)divider);
				noteDuration *= 1.5; // Increases the duration in half for dotted notes.
			}
			period = TO_MOD(endTune[i]);
			TPM0->MOD = period;
			TPM0_C0V = dutyCycle(period, 4); //25% duty cycle
			osDelay((noteDuration)*5/9);
			TPM0->MOD = 0;
			TPM0_C0V = 0;
			osDelay((noteDuration)*25/81);
		}
		connect = CHANGE_VAR;
}
