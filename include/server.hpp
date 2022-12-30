#ifndef SERVER_HPP
#define SERVER_HPP

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


void setupServer();
void notFound(AsyncWebServerRequest *request);
void setLEDparams(AsyncWebServerRequest *request);
void setLEDpreset(AsyncWebServerRequest *request);
void requestParameter(AsyncWebServerRequest *request);

#endif