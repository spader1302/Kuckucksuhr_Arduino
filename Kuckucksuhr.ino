/*!
 * @file Kuckucksuhr.ino
 * 
 * Main .ino File
 *
 * Author: Kevin Schauerte
 * 
 * last Update: 27.01.2020
 *
 */

#define IR_INPUT_PIN		3

//IR Receiver Library
#include "TinyIRReceiver.cpp.h"

//Einbindung der primären Klassen
#include "Clock.h"
#include "Display.h"
#include "UserInterface.h"
#include "Sound.h"
#include "Bluetooth.h"
#include "IRReceiver.h"

void setup() 
{	
	//Initialisierung der drei primären Klassen
	Clock.begin();
	Display.begin();
	UI.begin();
	BT.begin();	

	//Initialisierung IR-Lib
	initPCIInterruptForTinyReceiver();
}


void loop() 
{	
	//Update Uhrzeit mit allen zeitbedingten Abfragen
	Clock.update();

	//Aktualisierung Display bei User-Input	
	UI.update();

	//Update Sound
	Sound.update();

	//Listen to Bluetooth Commands
	BT.update();
}

//ISR für IR-Recevier
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat)
{
	Clock.disableAlarm();
	if (isRepeat) return;

	IR.setInput(aCommand);
}