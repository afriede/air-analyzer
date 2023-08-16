#pragma once
#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>

class Oled_Display {
private:
	Adafruit_SH1106G* af_sh1106g;
public:
	Oled_Display(Adafruit_SH1106G* af_sh1106g);
	void displayValue(int x, int y, const char* pretext, float val);
	void init();
	void clearDisplay();
	void display();
};
