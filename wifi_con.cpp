#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

void connectToWifi(String ssid, String password) {
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
}