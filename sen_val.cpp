#include "sen_val.h"
#include "sen_co2.h"
#include <SensirionI2CSen5x.h>

void updateSensorValues(SensorValues* sen, Sen_CO2* sen_co2, SensirionI2CSen5x* sen5x) {
	// sen->co2_ppm = sen_co2->getCO2();
	// uint16_t error;
	// char errorMessage[256];

	// // Read Measurement
	// float massConcentrationPm1p0;
	// float massConcentrationPm2p5;
	// float massConcentrationPm4p0;
	// float massConcentrationPm10p0;
	// float ambientHumidity;
	// float ambientTemperature;
	// float vocIndex;
	// float noxIndex;

	// error = sen5x->readMeasuredValues(
	// 	massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
	// 	massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
	// 	noxIndex);

	// if (error) {
	// 	Serial.print("Error trying to execute readMeasuredValues(): ");
	// 	errorToString(error, errorMessage, 256);
	// 	Serial.println(errorMessage);
	// } else {
	// 	sen->temp_c = ambientTemperature;
	// 	sen->hum_p_rel = ambientHumidity;
	// 	sen->mcpm1p0_ug_m3 = massConcentrationPm1p0;
	// 	sen->mcpm2p5_ug_m3 = massConcentrationPm2p5;
	// 	sen->mcpm4p0_ug_m3 = massConcentrationPm4p0;
	// 	sen->mcpm10p0_ug_m3 = massConcentrationPm10p0;
	// 	sen->VOC_index = vocIndex;
	// 	sen->NO2_index = noxIndex;
	// }
}