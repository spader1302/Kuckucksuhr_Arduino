/*!
 * @file Bluetooth.h
 *
 * Communication with Android App
 *
 */

#ifndef _BLUETOOTH_h
#define _BLUETOOTH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Clock.h"

#define BT_STATE_PIN			4
#define SERIAL_BAUDRATE			9600
#define MILLIS_WAITTIME			2000
#define MILLIS_SYNCDELAY		50
#define MAX_CLOCK_PAUSE_MILLIS	10000
#define START_BYTE				'X'
#define END_BYTE				'Z'

enum ConnectionState {
	NOT_CONNECTED,	
	CONNECTED,
	SYNCMODE
};

enum SyncState {
	STEP_1,
	STEP_2,
	STEP_3A,
	STEP_3B,
	STEP_4A,
	STEP_4B,
	STEP_5A,
	STEP_5B,
	STEP_6,
	STEP_7,
	STEP_8,
	STEP_9,
	STEP_10,
	STEP_11,
	FINISHED
};

class BluetoothClass
{
 public:

	 //Initialization
	void begin();

	//listen for incoming Commands, called by loop()
	void update();

	//sync only TimeData, called by Clock.updateTime() once every minute
	void syncTimeOnly();	

	//send Data to Android-App, all functions do nothing if cState = NOT_CONNECTED
	void sendNightmode(bool state);
	void sendHourly(bool state);
	void sendAlarmState(uint8_t alarm_no, bool state);
	void sendAlarmHour(uint8_t alarm_no, uint8_t hour);
	void sendAlarmMinute(uint8_t alarm_no, uint8_t minute);
	void sendTimeHour(uint8_t hour);
	void sendTimeMinute(uint8_t minute);
	void sendTimeSecond(uint8_t second);
	void sendTimeYear(uint16_t year);
	void sendTimeMonth(uint8_t month);
	void sendTimeDay(uint8_t day);

private:
	//Check if Android App is connected, changes cState to CONNECTED if succesful
	void queryConnection();	

	//send Command with START_BYTE and END_BYTE
	void sendCMD(String cmd);

	//read incoming Serial and check for valid Command
	bool readMessage();

	//called if incoming command is valid, execute command
	void handleReceived(String msg);

	//trigger all send* functions with MILLIS_SYNCDELAY inbetween
	void syncClockData();

	//send confirmation message, that a incoming command has been executed
	void confirmCmd();

	//Switch-Case Level 1	
	void nightInputCase(String msg);
	void hourlyInputCase(String msg);
	void alarmInputCase(String msg);
	void timeInputCase(String msg);

	//Switch-Case Level 2
	void alarmStateCase(String msg);
	void alarmHourCase(String msg);
	void alarmMinuteCase(String msg);
	void timeHourCase(String msg);
	void timeMinuteCase(String msg);
	void timeSecondCase(String msg);
	void timeYearCase(String msg);
	void timeMonthCase(String msg);
	void timeDayCase(String msg);

	ConnectionState cState = NOT_CONNECTED;
	SyncState sState = STEP_1;
	bool bEntry{ true };
	bool bSubEntry{ true };
	bool bClockPaused{ false };
	uint32_t lastMillis, millisLastUpdate, millisPausedAt;	
	uint16_t tempVal;
	String msgString, verifiedMessage;

};

extern BluetoothClass BT;

#endif

