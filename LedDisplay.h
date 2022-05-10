/*!
 * @file LedDisplay.h
 *
 * provides Support for LEDs on the ShiftRegister
 *
 */

#ifndef _LEDDISPLAY_h
#define _LEDDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ShiftRegister.h>
#include <RtcDateTime.h>

#define SR_STCLK_PIN			10
#define SR_DS_PIN				11
#define SR_SHCLK_PIN			13

#define ALARM_STATE_A			0b0000101010101010
#define ALARM_STATE_B			0b0000010101010101

class LedDisplayClass
{
 protected:


 public:

	 //Initialisierung
	void begin();

	//aktiviert LED-Anzeige
	void enable();

	//deaktiviert LED-Anzeige
	void disable();

	//wird regelm‰ﬂig aufgerufen, aktualisiert LED-Anzeige
	void update(RtcDateTime dt, bool bAlarmActive);

	//schaltet Alarm-Modus ein/aus
	void alarmMode(bool bAlarm);
	

private:

	void updateLEDs(RtcDateTime dt);
	void alarm();

	//Objekte
	ShiftRegister sr;

	//Variablen
	uint16_t led_state;
	uint8_t minute;
	uint8_t minutes_from_5;
	bool bEnabled{ true };
	bool blinkState;
};

extern LedDisplayClass LedDisplay;

#endif

