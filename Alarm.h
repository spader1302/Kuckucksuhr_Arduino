/*!
 * @file Alarm.h
 *
 * Defines a single Alarm Object
 *
 */

#ifndef _ALARM_h
#define _ALARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RtcDateTime.h>

class AlarmClass
{
 protected:


 public:

	//stellt Alarm-Uhrzeit ein	
	void set(RtcDateTime dt);

	//gibt eingestellte Alarm-Zeit zurück
	RtcDateTime get();

	//stellt ein, ob Alarm wiederholt wird oder einmalig ist
	void setRepeat(bool _bRepeat);

	//gibt zurück, ob Alarm wiederholt wird oder einmalig ist
	bool repeating();

	//Alarm aktivieren
	void enable();

	//Alarm deaktivieren
	void disable();

	//gibt Ein/Aus-Status zurück
	bool getState();

private:

	//Variablen
	RtcDateTime alarmTime;
	bool bRepeat;
	bool bEnabled = false;
};

extern AlarmClass Alarm;

#endif

