#include "server.hpp"
#include "ledstrip.hpp"

// =============================================================================
// Server details
// =============================================================================
AsyncWebServer server(80);
const char *PARAM_MESSAGE = "message";

// setup server
void setupServer()
{
    // landing page when connecting to ip
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world"); });

    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "roger"); });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, requestParameter);

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
              { String message;
                if (request->hasParam(PARAM_MESSAGE, true))
                    message = request->getParam(PARAM_MESSAGE, true)->value();
                else
                    message = "No message sent";
                request->send(200, "text/plain", "Hello, POST: " + message);
                Serial.print("Received msg /POST: ");
                Serial.println(message); });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
              { int params = request->params();
                for (int i = 0; i < params; i++)
                {
                    AsyncWebParameter *p = request->getParam(i);
                    if (p->isFile())
                        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size()); //p->isPost() is also true
                    else if (p->isPost())
                        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                    else
                        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                request->send(200, "text/plain", "Received Data"); });

    server.on("/led", HTTP_POST, setLEDparams);
    server.on("/preset", HTTP_POST, setLEDpreset);

    server.onNotFound(notFound);

    server.begin();
}

void notFound(AsyncWebServerRequest *request)
{
    Serial.println("something received but not found");
    Serial.println(request->contentLength());
    request->send(404, "text/plain", "Not found");
}

void setLEDparams(AsyncWebServerRequest *request)
{
    // STATUS
    bool status = true;
    if (request->hasParam("status", true))
    {
        status = request->getParam("status", true)->value().toInt();
        led_set_status(status);
    }

    // BRIGHTNESS
    uint8_t brightness = 0;
    if (request->hasParam("brightness", true))
    {
        brightness = request->getParam("brightness", true)->value().toInt();
        led_set_brightness(brightness);
    }

    uint8_t r = 0, g = 0, b = 0;
    bool colorReceived = false;
    // RED
    if (request->hasParam("r", true))
    {
        r = constrain(request->getParam("r", true)->value().toInt(), 0, 255);
        colorReceived = true;
    }

    // GREEN
    if (request->hasParam("g", true))
    {
        g = constrain(request->getParam("g", true)->value().toInt(), 0, 255);
        colorReceived = true;
    }

    // BLUE
    if (request->hasParam("b", true))
    {
        b = constrain(request->getParam("b", true)->value().toInt(), 0, 255);
        colorReceived = true;
    }
    if (colorReceived)
        led_set_color(r, g, b);

    Serial.print("LED INFO RECEIVED: ");
    Serial.printf("brightness: %d, r: %d, g: %d, b: %d\n", brightness, r, g, b);

    request->send(200, "text/plain", "roger"); // and let client know that we received in order
}

void setLEDpreset(AsyncWebServerRequest *request)
{
    // PRESET
    String message;
    if (request->hasParam("name", true))
    {
        message = request->getParam("name", true)->value();
        led_set_preset(message);
    }

    // PARAMETERS

    // FIRE
    if (message.equals("fire"))
    {
        uint8_t spark = 120, cool = 120, fps = 60, palno = 0;
        if (request->hasParam("spark", true))
            spark = request->getParam("spark", true)->value().toInt();
        if (request->hasParam("cool", true))
            cool = request->getParam("cool", true)->value().toInt();
        if (request->hasParam("fps", true))
            fps = request->getParam("fps", true)->value().toInt();
        if (request->hasParam("palno", true))
            palno = request->getParam("palno", true)->value().toInt();
        led_set_fire_parameters(spark, cool, fps, palno);
    }

    Serial.printf("PRESET CHANGED TO: %s\n", message.c_str());
    request->send(200, "text/plain", "roger"); // and let client know that we received in order
}

void requestParameter(AsyncWebServerRequest *request)
{
    Serial.print("GET request received: ");
    if (request->hasParam("brightness"))
    {
        request->send(200, "text/plain", String(led_get_brightness()));
        Serial.println("brightness");
    }
    else
    { // useless but keep for browser interaction : ip:port/get?message=test
        String message;
        if (request->hasParam(PARAM_MESSAGE, true))
            message = request->getParam(PARAM_MESSAGE, true)->value();
        else
            message = "No message sent";
        request->send(200, "text/plain", "Hello, POST: " + message);
    }
}