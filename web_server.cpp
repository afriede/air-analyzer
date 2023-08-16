#include "web_server.h"
#include "http.h"
#include "sen_val.h"

extern SensorValues sen;
extern WebServer server;

void initServer() {
	server.on("/", handleRoot);
	server.onNotFound(handleNotFound);
	server.begin();
}

void handleRoot() {
	String tmpString = html_1;
	tmpString.replace("%co2_val%", String(sen.co2_ppm));
	tmpString.replace("%temp_val%", String(sen.temp_c));
	tmpString.replace("%hum_val%", String(sen.hum_p_rel));
	tmpString.replace("%pm_val%", String(sen.mcpm2p5_ug_m3));
	tmpString.replace("%voxi_val%", String(sen.VOC_index));
	tmpString.replace("%no2i_val%", String(sen.NO2_index));
	server.send(200, "text/html", tmpString);
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
}