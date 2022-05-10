/*!
 * @file Alarm.cpp
 *
 * Defines a single Alarm Object
 *
 */

#include "Alarm.h"

void AlarmClass::set(RtcDateTime dt)
{
	alarmTime = dt;	
}

RtcDateTime AlarmClass::get()
{
	return alarmTime;
}

void AlarmClass::setRepeat(bool _bRepeat)
{
	bRepeat = _bRepeat;
}

bool AlarmClass::repeating()
{
	return bRepeat;
}

void AlarmClass::enable()
{
	bEnabled = true;
}

void AlarmClass::disable()
{
	bEnabled = false;
}

bool AlarmClass::getState()
{
	return bEnabled;
}


AlarmClass Alarm;
