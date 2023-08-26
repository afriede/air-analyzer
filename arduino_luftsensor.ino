#include "./creds.h"

// #include <WebServer.h>
// #include "web_server.h"
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "oled.h"

#include "sen_sensirion.h"
#include "sen_co2.h"
#include "sen_val.h"

#include "wifi_con.h"


#define WIDTH		128
#define HEIGHT		64

#define ONE_MINUTE	60

#define OLED_SCL	32
#define OLED_SDA	33
#define OLED_RST	25
#define OLED_DC		26
#define OLED_CS		27

Oled_Display oled(
	new Adafruit_SH1106G(WIDTH, HEIGHT, OLED_SDA, OLED_SCL, OLED_DC, OLED_RST, OLED_CS)
);
WiFiClient wifi;
PubSubClient mqtt(wifi);
Sen_CO2 sen_co2;
SensirionI2CSen5x sen5x;
SensorValues sen;
// WebServer server(80);


void setup() {
	connectToWifi(ssid, password);
	Serial.begin(115200);

	oled.init();
	mqtt.setServer("air.local", 1883);
	mqtt.connect("air-analyzer");
	if(mqtt.connected()){
		Serial.print("connected to mqtt!\n");
	}
	sen_co2.init();

	initSensirion(&sen5x);

	// initServer();

	Serial.println("ESP32 Initiated!");

}

u_int64_t t = 0;
uint8_t reconnect_attempt = 0;

void loop() {
	mqtt.loop();

	updateSensorValues(&sen, &sen_co2, &sen5x);

	oled.clearDisplay();

	oled.displayValue(0, 0,  "CO2: %7.0fppm", (float)sen.co2_ppm);
	oled.displayValue(0, 9,  "Temp: %6.2fC", (float)sen.temp_c);
	oled.displayValue(0, 18, "Hum: %7.2f%%", (float)sen.hum_p_rel);
	oled.displayValue(0, 27, "PM2.5: %5.2f", (float)sen.mcpm2p5_ug_m3);
	oled.displayValue(0, 36, "VOXi: %6.2f", (float)sen.VOC_index);
	oled.displayValue(0, 45, "NO2i: %6.2f", (float)sen.NO2_index);

	oled.display();

	if(t % ONE_MINUTE == 0){
		while(!mqtt.connected() && reconnect_attempt < 3) {
			reconnect_attempt++;
			Serial.printf("Attempt %d to reconnect to MQTT Broker!\n", reconnect_attempt);

			if(mqtt.connect("air-analyzer")) {
				break;
			}
			delay(1000);
		}
		reconnect_attempt = 0;
		if(mqtt.connected()){
			Serial.print("Send Values to MQTT!\n");
			char buffer[20];

			sprintf(buffer, "%d", sen.co2_ppm);
			mqtt.publish("sensor/co2", buffer);

			sprintf(buffer, "%6.2f", sen.temp_c);
			mqtt.publish("sensor/temp", buffer);

			sprintf(buffer, "%7.2f", sen.hum_p_rel);
			mqtt.publish("sensor/hum", buffer);

			sprintf(buffer, "%5.2f", sen.mcpm2p5_ug_m3);
			mqtt.publish("sensor/pm2_5", buffer);

			sprintf(buffer, "%6.2f", sen.VOC_index);
			mqtt.publish("sensor/voxi", buffer);

			sprintf(buffer, "%6.2f", sen.NO2_index);
			mqtt.publish("sensor/no2i", buffer);
		}
	}



	// server.handleClient();
	t = t+1;
	delay(1000);
}
