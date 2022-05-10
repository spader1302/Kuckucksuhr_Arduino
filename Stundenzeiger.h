/*!
 * @file Stundenzeiger.h
 *
 * provides Stepper Motor Tools
 *
 */

#ifndef _STUNDENZEIGER_h
#define _STUNDENZEIGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RtcDateTime.h>
#include <Stepper.h>
#include <EEPROM.h>

#define EEPROM_ADDRESS_POS		0x000

#define MINIMUM_STEP_SIZE		10
#define REVS_PER_MINUTE			5
#define STEPS_CORRECTION_NO		70		//vermutlich +/- 1
#define STEPS_PER_REVOLUTION	2064	//24 * 86		zwischen 2040 und 2064

#define STEPPER_IN1_PIN			A0
#define STEPPER_IN2_PIN			A1
#define STEPPER_IN3_PIN			A2
#define STEPPER_IN4_PIN			A3




class StundenzeigerClass
{
 protected:


 public:

	 //Initialisierung
	void begin();

	//bewegt den Zeiger zur angegebenen Uhrzeit ( Position )
	void moveTo(RtcDateTime dt);

	//wird zur Kalibrierung genutzt, stellt den gespeicherten Positionswert auf Null, Zeiger sollte auf 12 stehen
	void reset();

	//manuelle inkrementelle Bewegung des Zeigers im Uhrzeigersinn (MINIMUM_STEP_SIZE)
	void stepCW();

	//manuelle inkrementelle Bewegung des Zeigers gegen den Uhrzeigersinn (!!!!funktioniert nicht mit aktueller Bibliothek!!!!!)
	void stepCCW();

private:

	void moveAngle(uint8_t angle);

	//Korrektur nach vollständiger Umdrehung, da Step-Anzahl einer vollständigen Umdrehung nicht durch 24 teilbar ist
	void moveCorrectError();

	//speichert Zeigerposition im EEPROM
	void storePos();

	//lädt letzte Zeigerposition aus EEPROM
	uint8_t loadPos();

	//Variablen	
	int8_t old_pos{ 0 };
	int8_t new_pos{ 0 };
	uint8_t hour;
	
};

extern StundenzeigerClass Stundenzeiger;

#endif

