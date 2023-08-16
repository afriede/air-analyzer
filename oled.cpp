#include "oled.h"

Oled_Display::Oled_Display(Adafruit_SH1106G* af_sh1106g) {
	this->af_sh1106g = af_sh1106g;
}

void Oled_Display::displayValue(int x, int y, const char* pretext, float val) {
	this->af_sh1106g->setCursor(x, y);
	char* text = (char*)malloc(sizeof(char) * 20);
	sprintf(text, pretext, val);
	this->af_sh1106g->print(text);
	free(text);
}

void Oled_Display::init() {
	this->af_sh1106g->begin(0, true);
	this->af_sh1106g->setTextColor(SH110X_WHITE);
	this->af_sh1106g->setTextSize(1);
	this->af_sh1106g->clearDisplay();
	this->af_sh1106g->display();
}

void Oled_Display::clearDisplay() {
	this->af_sh1106g->clearDisplay();
}

void Oled_Display::display() {
	this->af_sh1106g->display();
}