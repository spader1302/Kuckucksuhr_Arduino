/*!
 * @file Clock.h
 *
 * Defines the main clock functionalities
 *
 */

#ifndef _Clock_h
#define _Clock_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//Libraries
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Stepper.h>
#include <ShiftRegister.h>
#include <EEPROM.h>

//eigene Klassen
#include "Display.h"
#include "Alarm.h"
#include "Stundenzeiger.h"
#include "LedDisplay.h"
#include "Kuckuck.h"
#include "Bluetooth.h"

#define RTC_RESET_PIN				7
#define RTC_DATA_PIN				8
#define RTC_CLK_PIN					9

#define INTERVAL_CLOCK_UPDATE		1000
#define NO_OF_ALARMS				2
#define TIME_ALARM_DEADZONE			60000

#define EEPROM_ADR_BHOURLY			0x001
#define EEPROM_ADR_BNIGHTMODE		0x002
#define EEPROM_ADR_STARTNIGHTHOUR	0x003
#define EEPROM_ADR_ENDNIGHTHOUR		0x004
#define EEPROM_ADR_ALARMLIST		0x008

class ClockClass
{
protected:


public:
	
	//Initialisierung
	void begin();

	//Alarm einstellen
	void setAlarm(uint8_t alarm_no, RtcDateTime dt, bool bRepeat);

	void update();

	//wird regelmäßig aufgerufen, aktualisiert Display, Stundezeiger, LEDs, etc.
	void updateTime(bool bUpdateClock);

	void pauseUpdate(bool pause);

	//gibt aktuelle Zeit zurück
	RtcDateTime getTime();

	//stündliches Signal On/Off
	void setHourly(bool bHourly);

	//gibt Status des stündlichen Signals zurück
	bool getHourly();

	//stellt den Nachtmodus an/aus sowie Start- und Endstunde
	void setNightmode(bool bNight, uint8_t start, uint8_t end);

	//gibt Ein/Aus-Status des Nachtmodus zurück
	bool getNightmode();

	//gibt Startstunde des Nachtmodus zurück (für Anzeige im Display)
	uint8_t getNightStart();

	//gibt Endstunde des Nachtmodus zurück (für Anzeige im Display)
	uint8_t getNightEnd();

	//schaltet den aktiven Alarm stumm
	void disableAlarm();

	//folgende Funktionen stellen Datum & Uhrzeit ein
	void setHour(uint8_t hour);
	void setMinute(uint8_t minute);
	void setSecond(uint8_t second);
	void setDay(uint8_t day);
	void setMonth(uint8_t month);
	void setYear(uint16_t year);

	//folgende Funktionen stellen Zeit der einzelnen Alarme ein
	void setAlarmHour(uint8_t alarm_no, uint8_t hour);
	void setAlarmMinute(uint8_t alarm_no, uint8_t minute);

	//schaltet den angegebenen Alarm ein/aus
	void setAlarmState(uint8_t alarm_no, bool state);

	//gibt Ein/Aus-Status des angegebenen Alarm zurück
	bool getAlarmState(uint8_t alarm_no);

	//gibt eingestellte Alarmzeit zurück
	RtcDateTime getAlarmTime(uint8_t alarm_no);

	//gibt zurück, ob laut Nachtmodus-Einstellungen gerade "Nacht" ist
	bool isNightTime();
	
	//Objekte
	AlarmClass alarmList[NO_OF_ALARMS]; //Array mit Alarm-Objekten (1 Objekt = 1 Alarm)

private:	

	//(de-)aktiviert Alarm-Signale
	void alarmSignal(bool bAlarmOn);

	//stellt Uhrzeit im RTC-Modul ein
	void setTime(RtcDateTime dt);

	//speichert Alarm-Objekt auf EEPROM
	void storeAlarm(uint8_t alarm_no);	

	//Variablen
	uint32_t lastSecond{ 0 };
	bool bUpdatePaused{ false };
	bool bHourlySignal, bNightmode, bNightTime;
	bool bAlarmEntry{ true };
	bool bAlarmActive = false;
	uint8_t startNightHour, endNightHour;
	uint32_t lastAlarm{ 0 };
	RtcDateTime now;
	RtcDateTime newTime;
	RtcDateTime alarmTime;
};

extern ClockClass Clock;

#endif

