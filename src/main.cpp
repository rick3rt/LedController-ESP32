#pragma GCC diagnostic ignored "-Wregister"

#include <Arduino.h>
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

#define DEBUG 1

// WIFI settings
const char *const ssid = "LekkerBrownennn2";
const char *const password = "wegwezen3";

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
    WiFi.begin(ssid, password);
    wifi_station_set_hostname("test11");
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
bool LED_status = false;
void loop()
{
    unsigned long cur_time = millis();

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

// =============================================================================
// OLD
// =============================================================================
// #define UPDATE_RATE 100
// LED strip updg && cur_time - last_LED_update >= 1000 / UPDATE_RATE)
// {ate
// if (isRunnin
//     last_LED_update = cur_time;
//     incrementColors();
// }
// #if 1
// #define NUM_GROUPS 3
// CRGB myPalette[NUM_GROUPS] = {
//     CRGB::Red,
//     CRGB::Orange,
//     CRGB::BlueViolet,
// };
// bool isRunning = false;
// #else
// #define NUM_GROUPS 15
// CRGB myPalette[NUM_GROUPS] = {
//     CRGB::Red,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Blue,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Green,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
//     CRGB::Black,
// };
// bool isRunning = true;
// #endif
// static void incrementColors()
// {
//     // multi color move
//     for (size_t i = NUM_LEDS - 1; i > 0; i--)
//         leds[i] = leds[i - 1];
//     firstLedCol = (firstLedCol + 1) % NUM_GROUPS;
//     leds[0] = myPalette[firstLedCol];
// }
//
// static void toggleRun()
// {
//     isRunning = !isRunning;
// }
