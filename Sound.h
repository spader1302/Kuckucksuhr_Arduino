/*!
 * @file Sound.h
 *
 * controls Sound played by Kuckuck and Alarm
 *
 */

#ifndef _SOUND_h
#define _SOUND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define AUDIO_SPEAKER_PIN		5

#define KUCKUCK_FREQ_1			1000
#define KUCKUCK_FREQ_2			400
#define KUCKUCK_DUR_1			300
#define KUCKUCK_DUR_2			200

#define ALARM_FREQ_1			300
#define ALARM_FREQ_2			800
#define ALARM_DUR_1				500
#define ALARM_DUR_2				500

enum SoundFile {
	KuckuckSound = 0,
	AlarmSound,
	NoSound
};

enum SoundStep {
	Step1 = 0,
	Step2
};

class SoundClass
{
 protected:


 public:
	void begin();
	void update();
	void playKuckuck();
	void playAlarm();
	void stop();

private:
	SoundFile soundPlaying{ NoSound };
	SoundStep currentStep{ Step1 };
	bool bEntry, bSubEntry;
	uint32_t startMillis{ 0 };


};

extern SoundClass Sound;

#endif

