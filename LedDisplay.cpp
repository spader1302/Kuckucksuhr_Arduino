/*!
 * @file LedDisplay.cpp
 *
 * provides Support for LEDs on the ShiftRegister
 *
 */

#include "LedDisplay.h"

void LedDisplayClass::begin()
{
	sr.init(SR_STCLK_PIN);
	sr.write2(0x000);
}

void LedDisplayClass::enable()
{
	sr.write2(led_state);
	bEnabled = true;
}

void LedDisplayClass::disable()
{
	led_state = sr.read();
	sr.write2(0x0000);
	bEnabled = false;
}

void LedDisplayClass::update(RtcDateTime dt, bool bAlarmActive)
{
	if (bAlarmActive) {
		alarm();
		return;
	}
	updateLEDs(dt);	
}

void LedDisplayClass::alarmMode(bool bAlarm)
{
	if (bAlarm) {
		bEnabled = false;
		
	}
	else {
		bEnabled = true;
		
	}
}

void LedDisplayClass::updateLEDs(RtcDateTime dt)
{
	minute = dt.Minute();
	minutes_from_5 = minute % 5;
	minute /= 5;

	//12 farbige LEDs
	for (uint8_t i{ 0 }; i < 12; ++i) {
		if (i == minute) {
			bitWrite(led_state, i, HIGH);
		}
		else {
			bitWrite(led_state, i, LOW);
		}
	}
	//4 weiße LEDs
	for (uint8_t i{ 0 }; i < 4; ++i) {
		if (minutes_from_5 > i) {
			bitWrite(led_state, i + 12, HIGH);
		}
		else {
			bitWrite(led_state, i + 12, LOW);
		}
	}

	//nur Schreiben falls LEDs ein
	if (bEnabled) {
		sr.write2(led_state);
	}
}

void LedDisplayClass::alarm()
{
	if (blinkState) {
		sr.write2(ALARM_STATE_A);
	}
	else {
		sr.write2(ALARM_STATE_B);
	}
	blinkState = !blinkState;
}




LedDisplayClass LedDisplay;

