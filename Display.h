/*!
 * @file Display.h
 *
 * provides Support the different Views in the 2x16 Display
 *
 */

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <LiquidCrystal_I2C.h>

#include "Clock.h"

#define DISPLAY_I2C_ADRESS		0x27
#define DISPLAY_WIDTH			16
#define DISPLAY_HEIGHT			2

enum View {
	Standard = 0,
	TimeMenu,
	Alarm1,
	Alarm2,
	HourlySetting,
	NightSetting,
	DateMenu,
	CalibrationMenu
};

enum SubView {
	Main = 0,
	OnOff,
	Stunde,
	Minute,
	Sekunde,
	Tag,
	Monat,
	Jahr,
	StartStunde,
	EndStunde,
	Calibration
};

class DisplayClass
{

 public:

	 //Initialisierung
	void begin();

	//aktualisiert letzte Display-Ansicht
	void update();

	//stellt Display-Ansicht entsprechend übergebener View und SubView ein
	void setView(View v, SubView subv);

	//leert das Display
	void clear();

	//Hintergrundbeleuchtung ein/aus
	void backlight(bool b);
	

private:

	//schaltet blinkenden Cursor ein/aus
	void cursor(bool bCursor);

	//bewegt Cursor (blinkend/nicht blinkend)
	void cursor(bool bCursor, uint8_t cursorPos);

	//Ausgabe des übergebenen Datums in formatiertem String
	void printDate(RtcDateTime dt);

	//Ausgabe der übergebenen Zeit in formatiertem String
	void printTime(RtcDateTime dt);

	//Ausgabe der übergebenen Stunde in formatiertem String
	void printHour(uint8_t hour);

	//die folgenden Funktionen beinhalten die verschiedenen Display-Ansichten
	void standardView();
	void timeMenuView(SubView subv);
	void alarmView(uint8_t alarm_no, SubView subv);
	void hourlySettingView(SubView subv);
	void nightSettingView(SubView subv);
	void dateMenuView(SubView subv);
	void calibrationView(SubView subv);

	//Variablen	
	uint8_t _cursorPos;		//0-15: Zeile 1, 16-31: Zeile 2
	char dateString[16];
	char timeString[9];
	char hourString[3];
	View lastV{ Standard };
	SubView lastSubv{ Main };
	RtcDateTime now;
	uint32_t lastMillis{ 0 };

	//Konstanten
	const char* wochentage[7]{ "So" , "Mo", "Di", "Mi", "Do", "Fr", "Sa" };

};

extern DisplayClass Display;

#endif

