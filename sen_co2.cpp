#include <HardwareSerial.h>
#include "sen_co2.h"

uint8_t getCheckSum(uint8_t *packet) {
	uint8_t checksum = 0;
	for(uint8_t i = 1; i < 8; i++) {
		checksum += packet[i];
	}
	checksum = 0xff - checksum + 1;
	return checksum;
}

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
	uint8_t msg_cs = retVal[8];
	uint8_t calc_cs = getCheckSum(retVal);

	// Serial.printf("MSG: %x %x %x %x %x %x %x %x %x\n", retVal[0],retVal[1],retVal[2],retVal[3],retVal[4],retVal[5],retVal[6],retVal[7],retVal[8]);
	// Serial.printf("Checksum: %x\n", calc_cs);
	free(retVal);
	if (msg_cs != calc_cs){
		return -1;
	}
	return ret;
}

Sen_CO2::~Sen_CO2() {
	Serial2.end();
}