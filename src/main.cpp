#include <Arduino.h>
#include <Hash.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <IPAddress.h>
#include <NTPClient.h>
#include <ESP8266mDNS.h>
#include <Adafruit_MPR121.h>
#include <CapacitiveSensor.h>
#include <PushButton.h>
//#include <RtcDateTime.h>
//#include <RtcDS3231.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>
#include <WString.h>
#include <cstdint>


#include "OTA.h"
struct{
	int filterPeriods;
	int filterLength;

}config;
void doReconfig(Button& btn, uint16_t duration);

OTA ota = OTA();
#define DEBUG_ESP_WIFI 1
#define DEBUG_ESP_PORT 1
const uint8_t RESET_BUTTON = 0;
//RtcDS3231 rtc;
//const uint8_t
PushButton button = PushButton(RESET_BUTTON);
NTPClient timeClient;
void setup() {
	Serial.begin(115200);
	Serial.print("Connecting...");
	WiFiManager wm;
	wm.autoConnect();
	Serial.println("Done.");
	ota.setup();
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	button.onHold(1500, doReconfig);
//	rtc.Begin();
	timeClient.forceUpdate();
	timeClient.getFormattedTime();
//	rtc.SetDateTime(RtcDateTime());
}
void doReconfig(Button& btn, uint16_t duration) {
	WiFiManager wifiManager;
	//reset settings - for testing
	//wifiManager.resetSettings();
	//wifiManager.setTimeout(120);
	String ssid = "ESP" + String(ESP.getChipId());
	if (!wifiManager.startConfigPortal(ssid.c_str())) {
		Serial.println("failed to connect and hit timeout");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(5000);
	}
	//if you get here you have connected to the WiFi
	Serial.println("connected.");

}

void loop() {
	ota.loop();
	timeClient.update();
}
