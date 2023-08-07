#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <SensirionI2CSen5x.h>
#include <Wire.h>

#include "./creds.h"
#include "./http.h"


#define WIDTH		128
#define HEIGHT		64

#define OLED_SCL	32
#define OLED_SDA	33
#define OLED_RST	25
#define OLED_DC		26
#define OLED_CS		27

#define RXD2 16
#define TXD2 17

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

WebServer server(80);
Adafruit_SH1106G oled(WIDTH, HEIGHT, OLED_SDA, OLED_SCL, OLED_DC, OLED_RST, OLED_CS);
String tmpString = "";
SensorValues sen;
SensirionI2CSen5x sen5x;

void handleRoot() {
	tmpString = html_1;
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

void displayValue(int x, int y, const char* pretext, float val) {
	oled.setCursor(x, y);
	char* text = (char*)malloc(sizeof(char) * 20);
	sprintf(text, pretext, val);
	oled.print(text);
	free(text);
}

void initOLED() {
	oled.begin(0, true);
	oled.setTextColor(SH110X_WHITE);
	oled.setTextSize(1);
	oled.clearDisplay();
	oled.display();
}

void initSensirion() {
	Wire.begin(19, 18);

	sen5x.begin(Wire);

	uint16_t error;
	char errorMessage[256];
	error = sen5x.deviceReset();
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
	error = sen5x.setTemperatureOffsetSimple(tempOffset);
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
	error = sen5x.startMeasurement();
	if (error) {
		Serial.print("Error trying to execute startMeasurement(): ");
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	}
}

int getCO2() {
	Serial2.write(0xFF);
	Serial2.write(0x01);
	Serial2.write(0x86);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x00);
	Serial2.write(0x79);

	Serial2.flush();
	// delay(100);
	byte* retVal = (byte*)malloc(9);
	Serial2.readBytes(retVal, 9);
	int ret = retVal[2] * 256 + retVal[3];
	free(retVal);

	return ret;
}

void updateSensorValues() {
	sen.co2_ppm = getCO2();
	uint16_t error;
	char errorMessage[256];

	// Read Measurement
	float massConcentrationPm1p0;
	float massConcentrationPm2p5;
	float massConcentrationPm4p0;
	float massConcentrationPm10p0;
	float ambientHumidity;
	float ambientTemperature;
	float vocIndex;
	float noxIndex;

	error = sen5x.readMeasuredValues(
		massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
		massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
		noxIndex);

	if (error) {
		Serial.print("Error trying to execute readMeasuredValues(): ");
		errorToString(error, errorMessage, 256);
		Serial.println(errorMessage);
	} else {
		sen.temp_c = ambientTemperature;
		sen.hum_p_rel = ambientHumidity;
		sen.mcpm1p0_ug_m3 = massConcentrationPm1p0;
		sen.mcpm2p5_ug_m3 = massConcentrationPm2p5;
		sen.mcpm4p0_ug_m3 = massConcentrationPm4p0;
		sen.mcpm10p0_ug_m3 = massConcentrationPm10p0;
		sen.VOC_index = vocIndex;
		sen.NO2_index = noxIndex;
	}
}

void setup() {

	Serial.begin(115200);

	// OLED init
	initOLED();

	// CO2 Sensor init
	Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

	// Sensirion init
	initSensirion();

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

	server.on("/", handleRoot);

	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	updateSensorValues();

	oled.clearDisplay();
	displayValue(0, 0, "CO2: %4.0fppm", (float)sen.co2_ppm);
	displayValue(0, 9, "Temp: %2.2fC", (float)sen.temp_c);
	displayValue(0, 18, "Hum: %3.2f%%", (float)sen.hum_p_rel);
	displayValue(0, 27, "PM2.5: %4.2f", (float)sen.mcpm2p5_ug_m3);
	displayValue(0, 36, "VOXi: %4.2f", (float)sen.VOC_index);
	displayValue(0, 45, "NO2i: %4.2f", (float)sen.NO2_index);
	oled.display();

	server.handleClient();
	delay(2);
}
