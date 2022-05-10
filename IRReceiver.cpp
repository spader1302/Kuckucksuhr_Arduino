/*!
 * @file IRReceiver.cpp
 *
 * IR Remote Control support
 *
 */

#include "IRReceiver.h"

void IRReceiverClass::setInput(uint8_t cmd)
{
	switch (ButtonCode(cmd)) {
	case FORWARD_BUTTON:
		UI.read('V');		
		break;

	case BACKWARD_BUTTON:
		UI.read('R');
		break;

	case ENTER_BUTTON:
		UI.read('E');
		break;

	case PLUS_BUTTON:
		UI.read('P');
		break;

	case MINUS_BUTTON:
		UI.read('M');
		break;

	default: 
		UI.read('X');
		break;
	}
}

IRReceiverClass IR;

