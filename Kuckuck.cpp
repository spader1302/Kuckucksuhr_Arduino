/*!
 * @file Kuckuck.cpp
 *
 * provides Kuckuck with servo functionality
 *
 */

#include "Kuckuck.h"


void KuckuckClass::begin()
{
	servo.attach(SERVO_PWM_PIN);
	servo.write(SERVO_IN_POS);
}

void KuckuckClass::moveOut()
{
	servo.write(SERVO_OUT_POS);	
	Sound.playKuckuck();
}	

void KuckuckClass::moveIn()
{	
	servo.write(SERVO_IN_POS);	
}

KuckuckClass Kuckuck;

