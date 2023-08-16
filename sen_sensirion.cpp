#include "sen_sensirion.h"
#include <Wire.h>

void initSensirion(SensirionI2CSen5x* sen5x) {
	Wire.begin(19, 18);

	sen5x->begin(Wire);

	uint16_t error;
	char errorMessage[256];
	error = sen5x->deviceReset();
	if (error) {
		Serial.print("Error trying to execute deviceReset(): ");
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	}

	// Print SEN55 module information if i2c buffers are large enough
#ifdef USE_PRODUCT_INFO
	printSerialNumber();
	printModuleVersions();
#endif

	// set a temperature offset in degrees celsius
	// Note: supported by SEN54 and SEN55 sensors
	// By default, the temperature and humidity outputs from the sensor
	// are compensated for the modules self-heating. If the module is
	// designed into a device, the temperature compensation might need
	// to be adapted to incorporate the change in thermal coupling and
	// self-heating of other device components.
	//
	// A guide to achieve optimal performance, including references
	// to mechanical design-in examples can be found in the app note
	// “SEN5x – Temperature Compensation Instruction” at www.sensirion.com.
	// Please refer to those application notes for further information
	// on the advanced compensation settings used
	// in `setTemperatureOffsetParameters`, `setWarmStartParameter` and
	// `setRhtAccelerationMode`.
	//
	// Adjust tempOffset to account for additional temperature offsets
	// exceeding the SEN module's self heating.
	float tempOffset = 0.0;
	error = sen5x->setTemperatureOffsetSimple(tempOffset);
	if (error) {
		Serial.print("Error trying to execute setTemperatureOffsetSimple(): ");
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	} else {
		Serial.print("Temperature Offset set to ");
		Serial.print(tempOffset);
		Serial.println(" deg. Celsius (SEN54/SEN55 only");
	}

	// Start Measurement
	error = sen5x->startMeasurement();
	if (error) {
		Serial.print("Error trying to execute startMeasurement(): ");
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	}
}