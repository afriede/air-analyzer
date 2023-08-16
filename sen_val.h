#pragma once
#include "sen_co2.h"
#include "sen_sensirion.h"


typedef struct {
	int co2_ppm;
	float mcpm1p0_ug_m3;
	float mcpm2p5_ug_m3;
	float mcpm4p0_ug_m3;
	float mcpm10p0_ug_m3;
	float hum_p_rel;
	float temp_c;
	float VOC_index;
	float NO2_index;
} SensorValues;

void updateSensorValues(SensorValues* sen, Sen_CO2* sen_co2, SensirionI2CSen5x* sen5x);