/*!
 * @file Kuckuck.h
 *
 * provides Kuckuck with servo functionality
 *
 */

#ifndef _KUCKUCK_h
#define _KUCKUCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>
#include "Sound.h"

#define SERVO_PWM_PIN			6

#define SERVO_OUT_POS			0
#define SERVO_IN_POS			180

class KuckuckClass
{
 protected:


 public:

	//Initialisierung
	void begin();

	//Bewege Kuckuck raus
	void moveOut();

	//Bewege Kuckuck wieder rein
	void moveIn();

private:
	Servo servo;
};

extern KuckuckClass Kuckuck;

#endif

