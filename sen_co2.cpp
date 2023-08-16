#include <HardwareSerial.h>
#include "sen_co2.h"

void Sen_CO2::init() {
	Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

int Sen_CO2::getCO2() {
	Serial2.write(0xFF);
	Serial2.write(0x01);
	Serial2.write(0x86);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x79);

	Serial2.flush();
	// delay(100);
	uint8_t* retVal = (uint8_t*)malloc(9);
	Serial2.readBytes(retVal, 9);
	int ret = retVal[2] * 256 + retVal[3];
	free(retVal);

	return ret;
}

Sen_CO2::~Sen_CO2() {
	Serial2.end();
}