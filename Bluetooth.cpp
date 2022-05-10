/*!
 * @file Bluetooth.cpp
 *
 * Communication with Android App
 *
 */

#include "Bluetooth.h"

void BluetoothClass::begin()
{
	Serial.begin(SERIAL_BAUDRATE);	
}

void BluetoothClass::update()
{
	if (!Serial || !digitalRead(BT_STATE_PIN)) {
		cState = NOT_CONNECTED;
		return;
	}

	switch (cState) {	

	case NOT_CONNECTED:
		queryConnection();		
		break;

	case CONNECTED:		
		if (readMessage()) {
			handleReceived(msgString);
		}
		if (bClockPaused && millis() > millisPausedAt + MAX_CLOCK_PAUSE_MILLIS) {
			bClockPaused = false;
			Clock.pauseUpdate(false);
		}
		break;

	case SYNCMODE:
		syncClockData();
		break;
	}	
}

void BluetoothClass::syncTimeOnly()
{
	if (cState != CONNECTED) return;
	cState = SYNCMODE;
	sState = STEP_6;
	bEntry = true;	
}

void BluetoothClass::sendNightmode(bool state)
{
	if (cState == NOT_CONNECTED) return;
	if (state) {
		sendCMD("N1");
	}
	else {
		sendCMD("N0");
	}
}

void BluetoothClass::sendHourly(bool state)
{
	if (cState == NOT_CONNECTED) return;
	if (state) {
		sendCMD("H1");
	}
	else {
		sendCMD("H0");
	}
}

void BluetoothClass::sendAlarmState(uint8_t alarm_no, bool state)
{
	if (cState == NOT_CONNECTED) return;
	if (state) {
		sendCMD("AS" + String(alarm_no) + "1");
	}
	else {
		sendCMD("AS" + String(alarm_no) + "0");
	}
}

void BluetoothClass::sendAlarmHour(uint8_t alarm_no, uint8_t hour)
{
	if (cState == NOT_CONNECTED) return;
	if (hour < 10) {
		sendCMD("AH" + String(alarm_no) + "0" + String(hour));
	}
	else {
		sendCMD("AH" + String(alarm_no) + String(hour));
	}
}

void BluetoothClass::sendAlarmMinute(uint8_t alarm_no, uint8_t minute)
{
	if (cState == NOT_CONNECTED) return;
	if (minute < 10) {
		sendCMD("AM" + String(alarm_no) + "0" + String(minute));
	}
	else {
		sendCMD("AM" + String(alarm_no) + String(minute));
	}
}

void BluetoothClass::sendTimeHour(uint8_t hour)
{
	if (cState == NOT_CONNECTED) return;
	if (hour < 10) {
		sendCMD("TH0" + String(hour));
	}
	else {
		sendCMD("TH" + String(hour));
	}
}

void BluetoothClass::sendTimeMinute(uint8_t minute)
{
	if (cState == NOT_CONNECTED) return;
	if (minute < 10) {
		sendCMD("Tm0" + String(minute));
	}
	else {
		sendCMD("Tm" + String(minute));
	}
}

void BluetoothClass::sendTimeSecond(uint8_t second)
{
	if (cState == NOT_CONNECTED) return;
	if (second < 10) {
		sendCMD("TS0" + String(second));
	}
	else {
		sendCMD("TS" + String(second));
	}
}

void BluetoothClass::sendTimeYear(uint16_t year)
{
	if (cState == NOT_CONNECTED) return;
	sendCMD("TY" + String(year));
}

void BluetoothClass::sendTimeMonth(uint8_t month)
{
	if (cState == NOT_CONNECTED) return;
	if (month < 10) {
		sendCMD("TM0" + String(month));
	}
	else {
		sendCMD("TM" + String(month));
	}
}

void BluetoothClass::sendTimeDay(uint8_t day)
{
	if (cState == NOT_CONNECTED) return;
	if (day < 10) {
		sendCMD("TD0" + String(day));
	}
	else {
		sendCMD("TD" + String(day));
	}
}

void BluetoothClass::queryConnection()
{
	if (bEntry) {
		lastMillis = millis();
		sendCMD("C");
		bEntry = false;
	}	
	if (millis() > lastMillis + MILLIS_WAITTIME) {
		bEntry = true;
	}

	if (!readMessage()) return;
	
	if (msgString.charAt(1) == 'C') {
		sendCMD("K");
		cState = CONNECTED;			
	}	
}

void BluetoothClass::syncClockData()
{
	if (cState == NOT_CONNECTED) return;

	if (bEntry) {
		lastMillis = millis();
		bEntry = false;
	}

	if (millis() > lastMillis + MILLIS_SYNCDELAY) {
		bSubEntry = true;
	}

	switch (sState) {

	case STEP_1:
		if (bSubEntry) {
			sendNightmode(Clock.getNightmode());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_2;
		}		
		break;

	case STEP_2:
		if (bSubEntry) {
			sendHourly(Clock.getHourly());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_3A;
		}
		break;

	case STEP_3A:
		if (bSubEntry) {
			sendAlarmState(0, Clock.getAlarmState(0));
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_3B;
		}
		break;

	case STEP_3B:
		if (bSubEntry) {
			sendAlarmState(1, Clock.getAlarmState(1));
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_4A;
		}
		break;

	case STEP_4A:
		if (bSubEntry) {
			sendAlarmHour(0, Clock.getAlarmTime(0).Hour());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_4B;
		}
		break;

	case STEP_4B:
		if (bSubEntry) {
			sendAlarmHour(1, Clock.getAlarmTime(1).Hour());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_5A;
		}
		break;

	case STEP_5A:
		if (bSubEntry) {
			sendAlarmMinute(0, Clock.getAlarmTime(0).Minute());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_5B;
		}
		break;

	case STEP_5B:
		if (bSubEntry) {
			sendAlarmMinute(1, Clock.getAlarmTime(1).Minute());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_6;
		}
		break;

	case STEP_6:
		if (bSubEntry) {
			sendTimeHour(Clock.getTime().Hour());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_7;
		}
		break;

	case STEP_7:
		if (bSubEntry) {
			sendTimeMinute(Clock.getTime().Minute());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_9;
		}
		break;

		//-------------------------------skipped, because unnecessary

	case STEP_8:
		if (bSubEntry) {
			sendTimeSecond(Clock.getTime().Second());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_9;
		}
		break;

		//---------------------------------------------------------------

	case STEP_9:
		if (bSubEntry) {
			sendTimeYear(Clock.getTime().Year());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_10;
		}
		break;

	case STEP_10:
		if (bSubEntry) {
			sendTimeMonth(Clock.getTime().Month());
			bSubEntry = false;
			lastMillis = millis();
			sState = STEP_11;
		}
		break;

	case STEP_11:
		if (bSubEntry) {
			sendTimeDay(Clock.getTime().Day());
			bSubEntry = false;
			lastMillis = millis();
			sState = FINISHED;
		}
		break;

	case FINISHED:
		if (bSubEntry) {
			cState = CONNECTED;
			sState = STEP_1;
			sendCMD("U");
		}		
		break;
	}	
}

void BluetoothClass::sendCMD(String cmd)
{
	Serial.println(START_BYTE + cmd + END_BYTE);	
}

bool BluetoothClass::readMessage()
{
	if (!Serial.available()) return false;

	msgString = Serial.readStringUntil('\n');	

	if (msgString.charAt(0) != START_BYTE) {
		//kein StartByte erkannt		
		return false;
	}
	if (msgString.charAt(msgString.length() - 1) != END_BYTE) {
		//kein EndByte erkannt		
		return false;
	}
	for (uint8_t i{ 0 }; i < msgString.length() - 1; ++i) {
		if (msgString.charAt(i) == END_BYTE) {
			//zu viele Commands auf einmal			
			return false;
		}
	}
	//Command korrekt
	verifiedMessage = String(msgString);		//überflüssig?
	return true;
}

void BluetoothClass::handleReceived(String msg)
{
	switch (msg.charAt(1)) {

	case 'N':
		nightInputCase(msg);
		break;

	case 'H':
		hourlyInputCase(msg);
		break;

	case 'A':
		alarmInputCase(msg);
		break;

	case 'T':
		timeInputCase(msg);
		break;

	case 'U':
		confirmCmd();
		cState = SYNCMODE;
		sState = STEP_1;
		bEntry = true;
		break;
	}
}

void BluetoothClass::confirmCmd()
{
	sendCMD("Q");
}

void BluetoothClass::nightInputCase(String msg)
{
	switch (msg.charAt(2)) {
	case '0':
		confirmCmd();
		Display.backlight(true);
		LedDisplay.enable();		
		break;

	case '1':
		confirmCmd();
		Display.backlight(false);
		LedDisplay.disable();	
		break;
	}
}

void BluetoothClass::hourlyInputCase(String msg)
{
	switch (msg.charAt(2)) {
	case '0':
		confirmCmd();
		Clock.setHourly(false);
		break;

	case '1':
		confirmCmd();
		Clock.setHourly(true);
		break;
	}
}

void BluetoothClass::alarmInputCase(String msg)
{
	switch (msg.charAt(2)) {
	case 'S':
		alarmStateCase(msg);
		break;

	case 'H':
		alarmHourCase(msg);
		break;

	case 'M':
		alarmMinuteCase(msg);
		break;
	}
}

void BluetoothClass::timeInputCase(String msg)
{
	switch (msg.charAt(2)) {
	case 'H':
		timeHourCase(msg);
		break;

	case 'm':
		timeMinuteCase(msg);
		break;

	case 'S':
		timeSecondCase(msg);
		break;

	case 'Y':
		timeYearCase(msg);
		break;

	case 'M':
		timeMonthCase(msg);
		break;

	case 'D':
		timeDayCase(msg);
		break;
	}
}

void BluetoothClass::alarmStateCase(String msg)
{
	switch (msg.charAt(4)) {
	case '0':
		confirmCmd();
		Clock.setAlarmState(msg.charAt(3) - '0', false);
		break;

	case '1':
		confirmCmd();
		Clock.setAlarmState(msg.charAt(3) - '0', true);
		break;
	}
}

void BluetoothClass::alarmHourCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(4) - '0') * 10 + msg.charAt(5) - '0';
	Clock.setAlarmHour(msg.charAt(3) - '0', tempVal);
}

void BluetoothClass::alarmMinuteCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(4) - '0') * 10 + msg.charAt(5) - '0';
	Clock.setAlarmMinute(msg.charAt(3) - '0', tempVal);
}

void BluetoothClass::timeHourCase(String msg)
{
	confirmCmd();
	Clock.pauseUpdate(true);
	bClockPaused = true;
	millisPausedAt = millis();
	tempVal = (msg.charAt(3) - '0') * 10 + msg.charAt(4) - '0';
	Clock.setHour(tempVal);	
}

void BluetoothClass::timeMinuteCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(3) - '0') * 10 + msg.charAt(4) - '0';
	Clock.setMinute(tempVal);
}

void BluetoothClass::timeSecondCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(3) - '0') * 10 + msg.charAt(4) - '0';
	Clock.setSecond(tempVal);
	Clock.pauseUpdate(false);
	bClockPaused = false;
}

void BluetoothClass::timeYearCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(3) - '0') * 1000 + (msg.charAt(4) - '0') * 100 + (msg.charAt(5) - '0') * 10 + msg.charAt(6) - '0';
	Clock.setYear(tempVal);
}

void BluetoothClass::timeMonthCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(3) - '0') * 10 + msg.charAt(4) - '0';	
	Clock.setMonth(tempVal);
}

void BluetoothClass::timeDayCase(String msg)
{
	confirmCmd();
	tempVal = (msg.charAt(3) - '0') * 10 + msg.charAt(4) - '0';
	Clock.setDay(tempVal);
}



BluetoothClass BT;
