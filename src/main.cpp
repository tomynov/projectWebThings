/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Originally based on the HelloServer example
 * Tested on ESP8266, ESP32, Arduino boards with WINC1500 modules (shields or
 * MKR1000)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"
#include <HCSR04.h> 
// #include "capteurMouv.h"

// TODO: Hardcode your wifi credentials here (and keep it private)
const char *ssid = "raspi-webgui";
const char *password = "laboiot20-21";
#define PinButton D6
#define ledPin D0
#define ledPin2 D3
#define Pinbuz D7
#define trigPin D1
#define echoPin D2
#define outPin D5
double lastValue = 0;

HCSR04 hc(trigPin, echoPin);

// capteurMouv c;

WebThingAdapter *adapter;

const char *ledTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led("led", "LED_move", ledTypes);
ThingProperty ledOn("on", "", BOOLEAN, "OnOffProperty");

const char *deviceTypes[] = {"MultiLevelSensor", "Sensor", nullptr};
ThingDevice device("AnalogSensorDevice", "Analog Sensor plugged in single pin", deviceTypes);
ThingProperty property("level", "Analog Input pin", NUMBER, "LevelProperty");

const char *btype[] = {"button", "Sensor", nullptr};
ThingDevice button("button", "Button_move", btype);
ThingProperty buttonDown("on", "", BOOLEAN, "OnOffProperty");


bool lastOn = false;

void afficher()
{

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("w25", WiFi.localIP());
  //led
  led.addProperty(&ledOn);
  adapter->addDevice(&led);
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led.id);
  // AnalogSensorDevice
  device.addProperty(&property);
  adapter->addDevice(&device);
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(device.id);
  //button
  button.addProperty(&buttonDown);
  adapter->addDevice(&button);
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(button.id);

  adapter->begin();
}

void setup(void)
{
  pinMode(PinButton, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  pinMode(Pinbuz,OUTPUT);
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(ledPin, HIGH); // active low led

  afficher();
}

void capteurMove() {
        // Led //
  bool on = ledOn.getValue().boolean;
  // active low led
  digitalWrite(ledPin, on );
  if (on != lastOn)
  {
    Serial.print(led.id);
    Serial.print(": ");
    Serial.println(on);
  }
  lastOn = on;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(100);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  ThingPropertyValue levelValue;
  levelValue.number = hc.dist();
  property.setValue(levelValue);
  {
    ThingPropertyValue buttonValue;
    buttonValue.boolean = digitalRead(PinButton);
    buttonDown.setValue(buttonValue);
  }
  if (hc.dist() > 20)
  {
    Serial.print("log: Value: ");
    Serial.println(hc.dist());

    //  lastValue = percent;
    delay(100);
  }
  else
  {
    Serial.print("log: Value: ");
    Serial.println(hc.dist());
    delay(100);
  }
}

void loop(void)
{
  analogWrite(Pinbuz,255);
  delay(50);
  analogWrite(Pinbuz,0);
  delay(10);
  capteurMove();
  adapter->update();
}
