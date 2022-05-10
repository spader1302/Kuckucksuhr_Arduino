/*!
 * @file UserInterface.cpp
 *
 * handles User Input and Menus
 *
 */

#include "UserInterface.h"


void UserInterfaceClass::begin()
{		
	pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
}

void UserInterfaceClass::read(char incCmd)
{	
	lastUserInput = millis();
	
	serial_in = incCmd;

	switch (v) {

	case Standard:
		standardCase();
		break;

	case TimeMenu:
		timeMenuCase();
		break;

	case Alarm1:
		alarmCase(0);
		break;

	case Alarm2:
		alarmCase(1);
		break;

	case HourlySetting:
		hourlySettingCase();		
		break;

	case NightSetting:
		nightSettingCase();		
		break;

	case DateMenu:
		dateMenuCase();		
		break;

	case CalibrationMenu:
		calibrationCase();		
		break;
	}	
}

void UserInterfaceClass::update()
{
	if (digitalRead(BUTTON_INPUT_PIN) == LOW) {
		Clock.disableAlarm();
		lastUserInput = millis();		
	}

	checkInactivity();

	if (bEntry) {
		bEntry = false;
		subv = Main;
		Clock.pauseUpdate(false);
		Display.clear();
		Display.setView(v, subv);
		lastUserInput = millis();
	}

	if (bSubEntry) {
		bSubEntry = false;
		Clock.pauseUpdate(true);
		Display.setView(v, subv);
	}
}

void UserInterfaceClass::checkInactivity()
{
	if (!Clock.getNightmode()) return;	

	if (millis() > lastUserInput + INTERVAL_DISPLAY_ON) {
		Display.backlight(false);
		LedDisplay.disable();
	}
	else {
		Display.backlight(true);
		LedDisplay.enable();
	}
}

void UserInterfaceClass::standardCase()
{
	switch (serial_in) {
	case 'V':
		v = TimeMenu;
		bEntry = true;
		break;

	case 'R':
		v = CalibrationMenu;
		bEntry = true;
		break;
	}
}

void UserInterfaceClass::timeMenuCase()
{
	switch (subv) {

	case Main:
		switch (serial_in) {
		case 'V':
			v = Alarm1;
			bEntry = true;
			break;

		case 'R':
			v = Standard;
			bEntry = true;
			break;

		case 'E':
			subv = Stunde;					
			bSubEntry = true;
			break;
		}
		break;

	case Stunde:
		switch (serial_in) {

		case 'V':
			subv = Minute;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incHour(ClockData);			
			break;

		case 'M':
			decHour(ClockData);
			break;

		}
		break;

	case Minute:
		switch (serial_in) {

		/*case 'V':
			subv = Sekunde;
			bSubEntry = true;
			break;
		*/
		case 'R':
			subv = Stunde;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incMinute(ClockData);
			break;

		case 'M':
			decMinute(ClockData);
			break;

		}
		break;

	case Sekunde:
		switch (serial_in) {

		case 'R':
			subv = Minute;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incSecond();
			break;

		case 'M':
			decSecond();
			break;

		}
		break;
	}
}

void UserInterfaceClass::alarmCase(uint8_t alarm_no)
{
	switch (subv) {

	case Main:
		switch (serial_in) {
		case 'V':
			if (alarm_no == 0) v = Alarm2;
			else v = HourlySetting;
			bEntry = true;
			break;

		case 'R':
			if (alarm_no == 0) v = TimeMenu;
			else v = Alarm1;
			bEntry = true;
			break;

		case 'E':
			subv = OnOff;
			bSubEntry = true;
			break;
		}
		break;

	case OnOff:
		switch (serial_in) {
		case 'V':
			subv = Stunde;
			bSubEntry = true;
			break;	

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			switchOnOff(AlarmSwitch, alarm_no);
			break;

		case 'M':
			switchOnOff(AlarmSwitch, alarm_no);
			break;
		}
		break;

	case Stunde:
		switch (serial_in) {

		case 'V':
			subv = Minute;
			bSubEntry = true;
			break;

		case 'R':
			subv = OnOff;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incHour(AlarmData, alarm_no);
			break;

		case 'M':
			decHour(AlarmData, alarm_no);
			break;

		}
		break;

	case Minute:
		switch (serial_in) {

		case 'R':
			subv = Stunde;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incMinute(AlarmData, alarm_no);
			break;

		case 'M':
			decMinute(AlarmData, alarm_no);
			break;
		}
		break;	

	}
}

void UserInterfaceClass::hourlySettingCase()
{
	switch (subv) {

	case Main:
		switch (serial_in) {
		case 'V':
			v = NightSetting;
			bEntry = true;
			break;

		case 'R':
			v = Alarm2;
			bEntry = true;
			break;

		case 'E':
			subv = OnOff;
			bSubEntry = true;
			break;
		}
		break;

	case OnOff:
		switch (serial_in) {

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			switchOnOff(HourlySwitch);
			break;

		case 'M':
			switchOnOff(HourlySwitch);
			break;
		}
		break;
	}
}

void UserInterfaceClass::nightSettingCase()
{
	switch (subv) {

	case Main:
		switch (serial_in) {
		case 'V':
			v = DateMenu;
			bEntry = true;
			break;

		case 'R':
			v = HourlySetting;
			bEntry = true;
			break;

		case 'E':
			subv = OnOff;
			bSubEntry = true;
			break;
		}
		break;

	case OnOff:
		switch (serial_in) {

		case 'V':
			subv = StartStunde;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			switchOnOff(NightSwitch);
			break;

		case 'M':
			switchOnOff(NightSwitch);
			break;
		}
		break;

	case StartStunde:

		switch (serial_in) {

		case 'V':
			subv = EndStunde;
			bSubEntry = true;
			break;

		case 'R':
			subv = OnOff;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incHour(NightStart);
			break;

		case 'M':
			decHour(NightStart);
			break;

		}
		break;

	case EndStunde:

		switch (serial_in) {

		case 'R':
			subv = StartStunde;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incHour(NightEnd);
			break;

		case 'M':
			decHour(NightEnd);
			break;

		}
		break;
	}
}

void UserInterfaceClass::dateMenuCase()
{
	switch (subv) {

	case Main:
		switch (serial_in) {
		case 'V':
			v = CalibrationMenu;
			bEntry = true;
			break;

		case 'R':
			v = NightSetting;
			bEntry = true;
			break;

		case 'E':
			subv = Tag;
			bSubEntry = true;
			break;
		}
		break;

	case Tag:
		switch (serial_in) {

		case 'V':
			subv = Monat;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incDay();
			break;

		case 'M':
			decDay();
			break;
		}
		break;

	case Monat:
		switch (serial_in) {

		case 'V':
			subv = Jahr;
			bSubEntry = true;
			break;

		case 'R':
			subv = Tag;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incMonth();
			break;

		case 'M':
			decMonth();
			break;
		}
		break;

	case Jahr:
		switch (serial_in) {

		case 'R':
			subv = Monat;
			bSubEntry = true;
			break;

		case 'E':
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			incYear();
			break;

		case 'M':
			decYear();
			break;
		}
		break;
	}
}

void UserInterfaceClass::calibrationCase()
{
	switch (subv) {

	case Main:

		switch (serial_in) {

		case 'V':
			v = Standard;
			bEntry = true;
			break;

		case 'R':
			v = DateMenu;
			bEntry = true;
			break;

		case 'E':
			subv = Calibration;
			bSubEntry = true;
			break;
		}
		break;

	case Calibration:
		switch (serial_in) {

		case 'E':
			Stundenzeiger.reset();
			v = Standard;
			bEntry = true;
			break;

		case 'P':
			Stundenzeiger.stepCW();
			break;

		case 'M':
			Stundenzeiger.stepCCW();
			break;
		}
	}
}

void UserInterfaceClass::incHour(TimeType tt)
{
	switch (tt) {
	case ClockData:		
		_tempTime = Clock.getTime();
		_tvar = _tempTime.Hour();		
		break;

	case NightStart:
		_tvar = Clock.getNightStart();		
		break;

	case NightEnd:
		_tvar = Clock.getNightEnd();
		break;
	}

	if (_tvar == 23) {
		_tvar = 0;
	}
	else {
		++_tvar;
	}

	switch (tt) {
	case ClockData:
		Clock.setHour(_tvar);
		Clock.updateTime(false);		
		break;

	case NightStart:
		Clock.setNightmode(Clock.getNightmode(), _tvar, Clock.getNightEnd());
		break;

	case NightEnd:
		Clock.setNightmode(Clock.getNightmode(), Clock.getNightStart(), _tvar);
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::decHour(TimeType tt)
{
	switch (tt) {
	case ClockData:
		_tempTime = Clock.getTime();
		_tvar = _tempTime.Hour();		
		break;

	case NightStart:
		_tvar = Clock.getNightStart();
		break;

	case NightEnd:
		_tvar = Clock.getNightEnd();
		break;
	}

	if (_tvar == 0) {
		_tvar = 23;
	}
	else {
		--_tvar;
	}

	switch (tt) {
	case ClockData:
		Clock.setHour(_tvar);
		Clock.updateTime(false);		
		break;

	case NightStart:
		Clock.setNightmode(Clock.getNightmode(), _tvar, Clock.getNightEnd());
		break;

	case NightEnd:
		Clock.setNightmode(Clock.getNightmode(), Clock.getNightStart(), _tvar);
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::incMinute(TimeType tt)
{
	switch (tt) {
	case ClockData:
		_tempTime = Clock.getTime();
		_tvar = _tempTime.Minute();
		if (_tvar == 59) {
			_tvar = 0;
		}
		else {
			++_tvar;
		}
		Clock.setMinute(_tvar);
		Clock.updateTime(false);		
		break;	
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::decMinute(TimeType tt)
{
	switch (tt) {
	case ClockData:
		_tempTime = Clock.getTime();
		_tvar = _tempTime.Minute();
		if (_tvar == 0) {
			_tvar = 59;
		}
		else {
			--_tvar;
		}
		Clock.setMinute(_tvar);
		Clock.updateTime(false);		
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::incHour(TimeType tt, uint8_t alarm_no)
{
	switch (tt) {
	case AlarmData:
		_tempTime = Clock.getAlarmTime(alarm_no);
		_tvar = _tempTime.Hour();
		if (_tvar == 23) {
			_tvar = 0;
		}
		else {
			++_tvar;
		}
		Clock.setAlarmHour(alarm_no, _tvar);		
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::decHour(TimeType tt, uint8_t alarm_no)
{
	switch (tt) {
	case AlarmData:
		_tempTime = Clock.getAlarmTime(alarm_no);
		_tvar = _tempTime.Hour();
		if (_tvar == 0) {
			_tvar = 23;
		}
		else {
			--_tvar;
		}
		Clock.setAlarmHour(alarm_no, _tvar);
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::incMinute(TimeType tt, uint8_t alarm_no)
{
	switch (tt) {
	case AlarmData:
		_tempTime = Clock.getAlarmTime(alarm_no);
		_tvar = _tempTime.Minute();
		if (_tvar == 59) {
			_tvar = 0;
		}
		else {
			++_tvar;
		}
		Clock.setAlarmMinute(alarm_no, _tvar);
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::decMinute(TimeType tt, uint8_t alarm_no)
{
	switch (tt) {
	case AlarmData:
		_tempTime = Clock.getAlarmTime(alarm_no);
		_tvar = _tempTime.Minute();
		if (_tvar == 0) {
			_tvar = 59;
		}
		else {
			--_tvar;
		}
		Clock.setAlarmMinute(alarm_no, _tvar);
		break;
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::incSecond()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Minute();
	if (_tvar == 59) {
		_tvar = 0;
	}
	else {
		++_tvar;
	}
	Clock.setSecond(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::decSecond()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Minute();
	if (_tvar == 0) {
		_tvar = 59;
	}
	else {
		--_tvar;
	}
	Clock.setSecond(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::incYear()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Year();
	if (_tvar == 3000) {
		_tvar = 2000;
	}
	else {
		++_tvar;
	}
	Clock.setYear(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::decYear()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Year();
	if (_tvar == 2000) {
		_tvar = 3000;
	}
	else {
		--_tvar;
	}
	Clock.setYear(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::incMonth()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Month();
	if (_tvar == 12) {
		_tvar = 1;
	}
	else {
		++_tvar;
	}
	Clock.setMonth(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::decMonth()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Month();
	if (_tvar == 1) {
		_tvar = 12;
	}
	else {
		--_tvar;
	}
	Clock.setMonth(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::incDay()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Day();
	if (_tvar == 31) {
		_tvar = 1;
	}
	else {
		++_tvar;
	}
	Clock.setDay(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::decDay()
{
	_tempTime = Clock.getTime();
	_tvar = _tempTime.Day();
	if (_tvar == 1) {
		_tvar = 31;
	}
	else {
		--_tvar;
	}
	Clock.setDay(_tvar);
	Clock.updateTime(false);
	Display.setView(v, subv);
}

void UserInterfaceClass::switchOnOff(SwitchType st)
{
	switch (st) {
	case HourlySwitch:
		Clock.setHourly(!Clock.getHourly());
		break;

	case NightSwitch:
		Clock.setNightmode(!Clock.getNightmode(), Clock.getNightStart(), Clock.getNightEnd());
	}
	Display.setView(v, subv);
}

void UserInterfaceClass::switchOnOff(SwitchType st, uint8_t alarm_no)
{
	switch (st) {
	case AlarmSwitch:
		Clock.setAlarmState(alarm_no, !Clock.getAlarmState(alarm_no));
		break;
	}
	Display.setView(v, subv);
}

UserInterfaceClass UI;

