/*!
 * @file UserInterface.h
 *
 * handles User Input and Menus
 *
 */

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Clock.h"
#include "Display.h"
#include "Stundenzeiger.h"

#define BUTTON_INPUT_PIN		2

#define INTERVAL_DISPLAY_ON		10000

enum TimeType {
	ClockData = 0,
	AlarmData,
	NightStart,
	NightEnd
};

enum SwitchType {
	AlarmSwitch,
	HourlySwitch,
	NightSwitch
};

class UserInterfaceClass
{
 public:

	 //Initialisierung
	void begin();

	//wird von loop() aufgerufen, steuert Anzeigen
	void update();

	//Abfrage nach User-Input mit entsprechender Zustandsmaschine. Gibt als Rückgabewert an, ob eine Eingabe vorhanden war
	void read(char incCmd);

private:

	//Display aus nach 5min (INTERVAL_DISPLAY_ON) Inaktivität
	void checkInactivity();

	//Aufteilung der Zustände in einzelne Funktionen zur Übersichtlichkeit
	void standardCase();
	void timeMenuCase();
	void alarmCase(uint8_t alarm_no);
	void hourlySettingCase();
	void nightSettingCase();
	void dateMenuCase();
	void calibrationCase();

	//Erhöhen / Verringern von Werten
	void incHour(TimeType tt);
	void decHour(TimeType tt);
	void incMinute(TimeType tt);
	void decMinute(TimeType tt);
	void incHour(TimeType tt, uint8_t alarm_no);
	void decHour(TimeType tt, uint8_t alarm_no);
	void incMinute(TimeType tt, uint8_t alarm_no);
	void decMinute(TimeType tt, uint8_t alarm_no);
	void incSecond();
	void decSecond();
	void incYear();
	void decYear();
	void incMonth();
	void decMonth();
	void incDay();
	void decDay();

	//Ein-/Ausschalten von Alarm, Nachtmodus, Stundensignal
	void switchOnOff(SwitchType st);
	void switchOnOff(SwitchType st, uint8_t alarm_no);

	//Variablen
	bool bEntry;
	bool bSubEntry;
	View v{ Standard };
	SubView subv{ Main };
	uint32_t lastUserInput{ 0 };
	char serial_in;
	RtcDateTime _tempTime;
	uint16_t _tvar;
};

extern UserInterfaceClass UI;

#endif

