/*!
 * @file IRReceiver.h
 *
 * IR Remote Control support
 *
 */

#ifndef _IRRECEIVER_h
#define _IRRECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "UserInterface.h"

enum ButtonCode {
	FORWARD_BUTTON = 0x40,
	BACKWARD_BUTTON = 0x44,
	ENTER_BUTTON = 0x43,
	PLUS_BUTTON = 0x15,
	MINUS_BUTTON = 0x7,
	NO1_BUTTON = 0xC,
	NO2_BUTTON = 0x18,
	NO3_BUTTON = 0x5E,
	NO4_BUTTON = 0x8,
	NO5_BUTTON = 0x1C,
	NO6_BUTTON = 0x5A,
	NO7_BUTTON = 0x42,
	NO8_BUTTON = 0x52,
	NO9_BUTTON = 0x4A,
	NO0_BUTTON = 0x16
};

class IRReceiverClass
{
 public:

	void setInput(uint8_t cmd);	

private:

	volatile uint8_t lastCmd;
	volatile bool bCmdReceived{ false };
	char cmdChar;
	ButtonCode buttonCode;
};

extern IRReceiverClass IR;

#endif

