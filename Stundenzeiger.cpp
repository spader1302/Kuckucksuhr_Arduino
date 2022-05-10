/*!
 * @file Stundenzeiger.cpp
 *
 * provides Stepper Motor Tools
 *
 */

#include "Stundenzeiger.h"

Stepper stepper(STEPS_PER_REVOLUTION, STEPPER_IN1_PIN, STEPPER_IN2_PIN, STEPPER_IN3_PIN, STEPPER_IN4_PIN);

void StundenzeigerClass::begin()
{
	old_pos = loadPos();
	stepper.setSpeed(REVS_PER_MINUTE);
}

void StundenzeigerClass::moveTo(RtcDateTime dt)
{
	if (dt.Hour() > 11) {
		hour = dt.Hour() - 12;
	}
	else {
		hour = dt.Hour();
	}

	new_pos = 2 * hour;

	if (dt.Minute() > 30) {
		++new_pos;
	}

	if (new_pos < old_pos) {
		moveAngle(new_pos + 23 - old_pos);		//move 1 Step less than required
		moveCorrectError();						//correction step is shorter than normal step
	}
	else {
		moveAngle(new_pos - old_pos);
	}	
	old_pos = new_pos;
	storePos();
}

void StundenzeigerClass::reset()
{
	old_pos = 0;
	new_pos = 0;
	storePos();
}

void StundenzeigerClass::stepCW()
{
	stepper.step(MINIMUM_STEP_SIZE);		
}

void StundenzeigerClass::stepCCW()
{
	stepper.step(- MINIMUM_STEP_SIZE);
}

void StundenzeigerClass::storePos()
{
	EEPROM.write(EEPROM_ADDRESS_POS, old_pos);	
}

uint8_t StundenzeigerClass::loadPos()
{
	uint8_t pos = EEPROM.read(EEPROM_ADDRESS_POS);	
	return pos;
}

void StundenzeigerClass::moveAngle(uint8_t angle)
{	
	stepper.step((angle * STEPS_PER_REVOLUTION) / 24);	
	digitalWrite(STEPPER_IN1_PIN, LOW);
	digitalWrite(STEPPER_IN2_PIN, LOW);
	digitalWrite(STEPPER_IN3_PIN, LOW);
	digitalWrite(STEPPER_IN4_PIN, LOW);

}

void StundenzeigerClass::moveCorrectError()
{
	stepper.step(STEPS_CORRECTION_NO);
}


StundenzeigerClass Stundenzeiger;

