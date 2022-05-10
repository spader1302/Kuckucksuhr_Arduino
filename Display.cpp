/*!
 * @file Display.cpp
 *
 * provides Support the different Views in the 2x16 Display
 *
 */

#include "Display.h"

LiquidCrystal_I2C lcd(DISPLAY_I2C_ADRESS, DISPLAY_WIDTH, DISPLAY_HEIGHT);

void DisplayClass::begin()
{	
	lcd.init();
}

void DisplayClass::update()
{
	setView(lastV, lastSubv);	
}

void DisplayClass::setView(View v, SubView subv)
{
	now = Clock.getTime();
	lastV = v;
	lastSubv = subv;

	switch (v) {

	case Standard:
		standardView();
		break;

	case TimeMenu:
		timeMenuView(subv);				
		break;

	case Alarm1:
		alarmView(0, subv);		
		break;

	case Alarm2:
		alarmView(1, subv);
		break;

	case HourlySetting:
		hourlySettingView(subv);		
		break;

	case NightSetting:
		nightSettingView(subv);		
		break;

	case DateMenu:
		dateMenuView(subv);		
		break;

	case CalibrationMenu:
		calibrationView(subv);		
		break;
	}
}

void DisplayClass::clear() 
{
	lcd.clear();
}

void DisplayClass::backlight(bool b) 
{
	if (b) {
		lcd.backlight();
	}
	else {
		lcd.noBacklight();
	}
}


void DisplayClass::cursor(bool bCursor)
{
	if (bCursor) {
		lcd.blink();
	}
	else {
		lcd.noBlink();
	}
}

void DisplayClass::cursor(bool bCursor, uint8_t cursorPos)
{
	cursor(bCursor);

	if (cursorPos > 15) {
		lcd.setCursor(cursorPos - 16, 1);		
	}
	else {
		lcd.setCursor(cursorPos, 0);
	}	
}

void DisplayClass::printDate(RtcDateTime dt)
{
	snprintf(dateString,
		sizeof(dateString),
		"%02u.%02u.%04u - %2s",
		dt.Day(),
		dt.Month(),
		dt.Year(),
		wochentage[dt.DayOfWeek()]
	);
	lcd.print(dateString);
}

void DisplayClass::printTime(RtcDateTime dt)
{
	snprintf(timeString,
		sizeof(timeString),
		"%02u:%02u:%02u",
		dt.Hour(),
		dt.Minute(),
		dt.Second()
	);
	lcd.print(timeString);
}

void DisplayClass::printHour(uint8_t hour)
{
	snprintf(hourString,
		sizeof(hourString),
		"%02uh",
		hour
	);
	lcd.print(hourString);
}

void DisplayClass::standardView() 
{
	cursor(false, 0);
	printDate(now);
	cursor(false, 16);
	printTime(now);
}

void DisplayClass::timeMenuView(SubView subv)
{
	cursor(false, 0);
	lcd.print("Uhrzeit einst.");
	cursor(false, 16);
	printTime(now);

	switch (subv) {

	case Stunde:
		cursor(true, 17);		
		break;

	case Minute:
		cursor(true, 20);
		break;

	case Sekunde:
		cursor(true, 23);
		break;
	}
}

void DisplayClass::alarmView(uint8_t alarm_no, SubView subv)
{
	cursor(false, 0);
	lcd.print("Alarm ");
	cursor(false, 6);
	lcd.print(alarm_no + 1);
	cursor(false, 7);
	lcd.print(':');
	cursor(false, 16);
	printTime(Clock.getAlarmTime(alarm_no));

	switch (subv) {

	case Main:
		cursor(false, 9);
		if (Clock.getAlarmState(alarm_no)) {
			lcd.print("An");
		}
		else {
			lcd.print("Aus");
		}
		break;

	case OnOff:
		cursor(false, 9);
		lcd.print("   ");
		cursor(false, 9);
		if (Clock.getAlarmState(alarm_no)) {
			lcd.print("An");
		}
		else {
			lcd.print("Aus");
		}
		cursor(true, 9);		
		break;

	case Stunde:
		cursor(true, 17);		
		break;

	case Minute:
		cursor(true, 20);		
		break;

	}
}

void DisplayClass::hourlySettingView(SubView subv)
{
	cursor(false, 0);
	lcd.print("Stundensignal:");
	cursor(false, 16);	

	switch (subv) {	

	case Main:				
		if (Clock.getHourly()) {
			lcd.print("On");
		}
		else {
			lcd.print("Off");
		}
		break;

	case OnOff:	
		lcd.print("   ");
		cursor(false, 16);
		if (Clock.getHourly()) {
			lcd.print("On");
		}
		else {
			lcd.print("Off");
		}
		cursor(true, 16);
		break;
	}
}

void DisplayClass::nightSettingView(SubView subv)
{
	cursor(false, 0);
	lcd.print("Nachtmodus:");	

	switch (subv) {

	case Main:
		cursor(false, 16);
		if (Clock.getNightmode()) {
			lcd.print("On");
		}
		else {
			lcd.print("Off");
		}
		break;

	case OnOff:
		cursor(false, 16);
		lcd.print("   ");
		cursor(false, 16);
		if (Clock.getNightmode()) {
			lcd.print("On");
		}
		else {
			lcd.print("Off");
		}
		cursor(true, 16);
		break;

	case StartStunde:
		cursor(false, 16);
		lcd.print("Start: ");
		cursor(false, 23);
		printHour(Clock.getNightStart());
		cursor(true, 24);
		break;

	case EndStunde:
		cursor(false, 16);
		lcd.print("Ende: ");
		cursor(false, 23);
		printHour(Clock.getNightEnd());
		cursor(true, 24);
		break;
	}
}

void DisplayClass::dateMenuView(SubView subv) 
{
	cursor(false, 0);
	lcd.print("Datum einst.");
	cursor(false, 16);
	printDate(now);

	switch (subv) {

	case Tag:
		cursor(true, 17);
		break;

	case Monat:
		cursor(true, 20);
		break;

	case Jahr:
		cursor(true, 25);
		break;
	}
}

void DisplayClass::calibrationView(SubView subv)
{
	cursor(false, 0);

	switch (subv) {

	case Main:
		lcd.print("Kalibrierung");
		break;

	case Calibration:
		lcd.print("Zeiger auf 12");
		cursor(true, 16);
		lcd.print("Best\xE1tigen");
		break;
	}
}


DisplayClass Display;

