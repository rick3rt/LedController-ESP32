#include <Arduino.h>
#if 0
#define PIN 14
void setup()
{
    pinMode(PIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("hallo");
}

void loop()
{
    Serial.println("high");
    digitalWrite(PIN, HIGH);
    delay(500);

    Serial.println("low");
    digitalWrite(PIN, LOW);
    delay(500);
}

#else
#include <ArduinoOTA.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "webserial.hpp"
#include "ledstrip.hpp"
#include "server.hpp"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define DEBUG 0

// WIFI settings
const char *const ssid = "LekkerBrownennn2";
const char *const password = "wegwezen3";
const char *const hostname = "LED-Controller";

// =============================================================================
// webserial callback
// =============================================================================
void callback(unsigned char *data, unsigned int length)
{
    data[length] = '\0';
    Serial.println((char *)data);
}

// =============================================================================
// Setup
// =============================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nSerial monitor started");

    // start WIFI
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname);
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // set status led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // start server
    setupServer();
    Serial.println("SERVER online");

    led_setup();
    Serial.println("LED strip initialized");

    // OTA:
    ArduinoOTA.begin();

// webserial
#if USE_WEBSERIAL
    WebSerial.begin(&server);
    WebSerial.msgCallback(callback);
#endif
}

// =============================================================================
// Loop
// =============================================================================
unsigned long last_LED_update = 0;
unsigned long last_LED_switch = 0;
unsigned long last_debug_print = 0;
bool LED_status = false;
void loop()
{
    unsigned long cur_time = millis();

#if DEBUG
    if (cur_time - last_debug_print >= 1000)
    {
        last_debug_print = cur_time;
        Serial.println("looping");
    }
#endif

// builtin LED flash (debug)
#if DEBUG
    if (cur_time - last_LED_switch >= 500)
    {
        last_LED_switch = cur_time;
        LED_status = !LED_status;              // toggle bool
        digitalWrite(LED_BUILTIN, LED_status); // and turn led off or on
    }
#endif

    led_update();

    // OTA:
    ArduinoOTA.handle();
}
#endif
