#include <Arduino.h>
#include <ArduinoOTA.h>

#define DEBUG 0


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


// WIFI settings
const char *const ssid = "LekkerBrownennn";
const char *const password = "wegwezen3";
const char *const hostname = "LED-Controller";
bool wifi_connected = false;

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

    led_setup();
    Serial.println("LED strip initialized");

    // set status led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // start WIFI
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname);
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {

        Serial.printf("WiFi Failed! Reason: %i\n", WiFi.status());
        return;
    }
    wifi_connected = true;

// webserial
#if USE_WEBSERIAL
    WebSerial.begin(&server);
    WebSerial.msgCallback(callback);
#endif

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // start server
    setupServer();
    Serial.println("SERVER online");

    // OTA:
    ArduinoOTA.begin();
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
    if (cur_time - last_debug_print >= 1000 * 3)
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

    if (wifi_connected)
    {
        // OTA:
        ArduinoOTA.handle();
    }
}
