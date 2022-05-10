/*!
 * @file Sound.cpp
 *
 * controls Sound played by Kuckuck and Alarm
 *
 */

#include "Sound.h"




void SoundClass::begin()
{
}

void SoundClass::update()
{
	switch (soundPlaying) {

	case NoSound:
		break;

	case KuckuckSound:
		if (bEntry) {
			startMillis = millis();
			bEntry = false;
			noTone(AUDIO_SPEAKER_PIN);
			bSubEntry = true;
		}		

		switch (currentStep) {		

		case Step1:
			if (bSubEntry) {
				startMillis = millis();
				tone(AUDIO_SPEAKER_PIN, KUCKUCK_FREQ_1, KUCKUCK_DUR_1);
				bSubEntry = false;				
			}	
			if (millis() - KUCKUCK_DUR_1 > startMillis) {
				noTone(AUDIO_SPEAKER_PIN);
				currentStep = Step2;				
				bSubEntry = true;
			}
			break;

		case Step2:
			if (bSubEntry) {
				startMillis = millis();
				tone(AUDIO_SPEAKER_PIN, KUCKUCK_FREQ_2, KUCKUCK_DUR_2);
				bSubEntry = false;				
			}			
			if (millis() - KUCKUCK_DUR_2 > startMillis) {
				noTone(AUDIO_SPEAKER_PIN);				
				soundPlaying = NoSound;		
			}
			break;
		}

		break;

	case AlarmSound:
		if (bEntry) {
			startMillis = millis();
			bEntry = false;
			noTone(AUDIO_SPEAKER_PIN);
			bSubEntry = true;
		}

		switch (currentStep) {

		case Step1:
			if (bSubEntry) {
				startMillis = millis();
				tone(AUDIO_SPEAKER_PIN, ALARM_FREQ_1, ALARM_DUR_1);
				bSubEntry = false;
			}
			if (millis() - ALARM_DUR_1 > startMillis) {
				noTone(AUDIO_SPEAKER_PIN);
				currentStep = Step2;
				bSubEntry = true;
			}
			break;

		case Step2:
			if (bSubEntry) {
				startMillis = millis();
				tone(AUDIO_SPEAKER_PIN, ALARM_FREQ_2, ALARM_DUR_2);
				bSubEntry = false;
			}
			if (millis() - ALARM_DUR_2 > startMillis) {
				noTone(AUDIO_SPEAKER_PIN);
				currentStep = Step1;
				bSubEntry = true;
			}
			break;
		}

		break;
	}
}

void SoundClass::playKuckuck()
{
	soundPlaying = KuckuckSound;
	currentStep = Step1;
	bEntry = true;	
}

void SoundClass::playAlarm()
{
	soundPlaying = AlarmSound;	
	currentStep = Step1;
	bEntry = true;
}

void SoundClass::stop()
{
	soundPlaying = NoSound;
	bEntry = true;
}


SoundClass Sound;

