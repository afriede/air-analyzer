#pragma once
#define RXD2 16
#define TXD2 17

class Sen_CO2 {
public:
	void init();
	int getCO2();
	~Sen_CO2();
};
