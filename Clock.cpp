/*!
 * @file Clock.cpp
 * 
 * Defines the main clock functionalities
 * 
 */ 


#include "Clock.h"


ThreeWire rtcWire(RTC_DATA_PIN, RTC_CLK_PIN, RTC_RESET_PIN);
RtcDS1302<ThreeWire> rtc(rtcWire);


void ClockClass::begin()
{
	//Initialisierung
	rtc.Begin();
	Stundenzeiger.begin();
	Kuckuck.begin();
	LedDisplay.begin();	

	//Lade Daten aus EEPROM
	bHourlySignal = EEPROM.read(EEPROM_ADR_BHOURLY);
	bNightmode = EEPROM.read(EEPROM_ADR_BNIGHTMODE);
	startNightHour = EEPROM.read(EEPROM_ADR_STARTNIGHTHOUR);
	endNightHour = EEPROM.read(EEPROM_ADR_ENDNIGHTHOUR);

	for (uint8_t i{ 0 }; i < NO_OF_ALARMS; ++i) {
		EEPROM.get(EEPROM_ADR_ALARMLIST + i * sizeof(AlarmClass), alarmList[i]);
	}
}

void ClockClass::setAlarm(uint8_t alarm_no, RtcDateTime dt, bool bRepeat)
{	
	alarmList[alarm_no].set(dt);
	alarmList[alarm_no].setRepeat(bRepeat);
	storeAlarm(alarm_no);
}

void ClockClass::update()
{
	now = rtc.GetDateTime();
	if (!bUpdatePaused && now.TotalSeconds() > lastSecond) {
		Clock.updateTime(true);
		lastSecond = now.TotalSeconds();
	}	
}

void ClockClass::pauseUpdate(bool pause)
{
	bUpdatePaused = pause;
}

void ClockClass::alarmSignal(bool bAlarmOn)
{
	LedDisplay.alarmMode(bAlarmOn);
}

void ClockClass::setTime(RtcDateTime dt)
{
	rtc.SetDateTime(dt);
	lastSecond = dt.TotalSeconds();
}

void ClockClass::storeAlarm(uint8_t alarm_no)
{
	EEPROM.put(EEPROM_ADR_ALARMLIST + alarm_no * sizeof(AlarmClass), alarmList[alarm_no]);
}

void ClockClass::updateTime(bool bUpdateClock)
{
	if (!bUpdateClock) {
		now = rtc.GetDateTime();
		return;
	}

	Display.update();

	//Nachtmodus an/aus?
	/*
	if (bNightmode) {
		if ((now.Hour() > startNightHour && now.Hour() < endNightHour) || (startNightHour > endNightHour && (now.Hour() > startNightHour || now.Hour() < endNightHour))) {
			bNightTime = true;
		}
		else {
			bNightTime = false;
		}
	}
	else {
		bNightTime = false;
	}
	*/

	//Alarmabfrage
	for (uint8_t i{ 0 }; i < NO_OF_ALARMS; ++i) {

		if (!alarmList[i].getState() || !bAlarmEntry || millis() - lastAlarm < TIME_ALARM_DEADZONE) break;

		alarmTime = alarmList[i].get();
		if (now.Hour() == alarmTime.Hour() && now.Minute() == alarmTime.Minute()) {
			bAlarmActive = true;
			lastAlarm = millis();
			bAlarmEntry = false;
			Sound.playAlarm();
		}
	}

	//stündliches Kuckuck-Signal
	if (bHourlySignal) {
		if (now.Minute() == 0) {
			if (now.Second() == 0) {
				Kuckuck.moveOut();
			}
			else if (now.Second() == 1) {
				Kuckuck.moveIn();
			}
		}
	}

	//Update Zeiger und LED-Display
	Stundenzeiger.moveTo(now);
	LedDisplay.update(now, bAlarmActive);

	//Update Bluetooth
	if (now.Second() == 0) {
		BT.syncTimeOnly();
	}
	

}

RtcDateTime ClockClass::getTime()
{
	return now;
}

void ClockClass::setHourly(bool bHourly)
{
	bHourlySignal = bHourly;
	EEPROM.write(EEPROM_ADR_BHOURLY, bHourlySignal);
}

bool ClockClass::getHourly()
{
	return bHourlySignal;
}

void ClockClass::setNightmode(bool bNight, uint8_t start, uint8_t end)
{
	bNightmode = bNight;
	startNightHour = start;
	endNightHour = end;
	EEPROM.write(EEPROM_ADR_BNIGHTMODE, bNightmode);
	EEPROM.write(EEPROM_ADR_STARTNIGHTHOUR, startNightHour);
	EEPROM.write(EEPROM_ADR_ENDNIGHTHOUR, endNightHour);

	if (bNight) {
		Display.backlight(false);
		LedDisplay.disable();
	}
	else {
		Display.backlight(true);
		LedDisplay.enable();
	}
}

bool ClockClass::getNightmode()
{
	return bNightmode;
}

uint8_t ClockClass::getNightStart()
{
	return startNightHour;
}

uint8_t ClockClass::getNightEnd()
{
	return endNightHour;
}

void ClockClass::disableAlarm()
{
	bAlarmActive = false;
	bAlarmEntry = true;
	Sound.stop();
}

void ClockClass::setHour(uint8_t hour)
{
	if (hour > 23) return;
	newTime = RtcDateTime(now.Year(), now.Month(), now.Day(), hour, now.Minute(), now.Second());
	setTime(newTime);	
}

void ClockClass::setMinute(uint8_t minute)
{
	if (minute > 59) return;
	newTime = RtcDateTime(now.Year(), now.Month(), now.Day(), now.Hour(), minute, now.Second());
	setTime(newTime);
}

void ClockClass::setSecond(uint8_t second)
{
	if (second > 59) return;
	newTime = RtcDateTime(now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), second);
	setTime(newTime);
}

void ClockClass::setDay(uint8_t day)
{
	if (day > 31) return;
	newTime = RtcDateTime(now.Year(), now.Month(), day, now.Hour(), now.Minute(), now.Second());
	setTime(newTime);
}

void ClockClass::setMonth(uint8_t month)
{
	if (month > 12) return;
	newTime = RtcDateTime(now.Year(), month, now.Day(), now.Hour(), now.Minute(), now.Second());
	setTime(newTime);
}

void ClockClass::setYear(uint16_t year)
{
	if (year < 2000 || year > 3000) return;
	newTime = RtcDateTime(year, now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
	setTime(newTime);	
}

void ClockClass::setAlarmHour(uint8_t alarm_no, uint8_t hour)
{
	if (hour > 23) return;
	newTime = getAlarmTime(alarm_no);
	newTime = RtcDateTime(newTime.Year(), newTime.Month(), newTime.Day(), hour, newTime.Minute(), 0);
	setAlarm(alarm_no, newTime, true);
}

void ClockClass::setAlarmMinute(uint8_t alarm_no, uint8_t minute)
{
	if (minute > 59) return;
	newTime = getAlarmTime(alarm_no);
	newTime = RtcDateTime(newTime.Year(), newTime.Month(), newTime.Day(), newTime.Hour(), minute, 0);
	setAlarm(alarm_no, newTime, true);
}

void ClockClass::setAlarmState(uint8_t alarm_no, bool state)
{
	if (state) {
		alarmList[alarm_no].enable();
	}
	else {
		alarmList[alarm_no].disable();
	}
	storeAlarm(alarm_no);
}

bool ClockClass::getAlarmState(uint8_t alarm_no)
{
	return alarmList[alarm_no].getState();	
}

RtcDateTime ClockClass::getAlarmTime(uint8_t alarm_no)
{	
	return alarmList[alarm_no].get();;
}

bool ClockClass::isNightTime()
{
	return bNightTime;
}


ClockClass Clock;

