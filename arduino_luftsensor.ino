#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "./creds.h"

#include "web_server.h"
#include "oled.h"

#include "sen_sensirion.h"
#include "sen_co2.h"
#include "sen_val.h"


#define WIDTH		128
#define HEIGHT		64

#define OLED_SCL	32
#define OLED_SDA	33
#define OLED_RST	25
#define OLED_DC		26
#define OLED_CS		27

Oled_Display oled(
	new Adafruit_SH1106G(WIDTH, HEIGHT, OLED_SDA, OLED_SCL, OLED_DC, OLED_RST, OLED_CS)
);
Sen_CO2 sen_co2;
SensirionI2CSen5x sen5x;
SensorValues sen;
WebServer server(80);


void setup() {

	Serial.begin(115200);

	oled.init();
	sen_co2.init();

	// Sensirion init
	initSensirion(&sen5x);

	// Wifi init
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	if (MDNS.begin("air-analyzer")) {
		Serial.println("MDNS responder started");
	}
	initServer();
	Serial.println("HTTP server started");
}

void loop() {
	// updateSensorValues(&sen, &sen_co2, &sen5x);

	oled.clearDisplay();
	oled.displayValue(0, 0, "CO2: %4.0fppm", (float)sen.co2_ppm);
	oled.displayValue(0, 9, "Temp: %2.2fC", (float)sen.temp_c);
	oled.displayValue(0, 18, "Hum: %3.2f%%", (float)sen.hum_p_rel);
	oled.displayValue(0, 27, "PM2.5: %4.2f", (float)sen.mcpm2p5_ug_m3);
	oled.displayValue(0, 36, "VOXi: %4.2f", (float)sen.VOC_index);
	oled.displayValue(0, 45, "NO2i: %4.2f", (float)sen.NO2_index);
	oled.display();

	server.handleClient();
	delay(2);
}
