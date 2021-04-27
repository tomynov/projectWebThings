/*
#include <Arduino.h>
#include <main.cpp>
#include "capteurMouv.h"


capteurMouv::~capteurMouv(){

};

void afficher(){

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    adapter = new WebThingAdapter("w25", WiFi.localIP());
    //led
    led.addProperty(&ledOn);
    adapter->addDevice(&led);
    adapter->begin();
    Serial.println("HTTP server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.print("/things/");
    Serial.println(led.id);
    //device
    device.addProperty(&property);
    adapter->addDevice(&device);
    adapter->begin();
    Serial.println("HTTP server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.print("/things/");
    Serial.println(device.id);
}

void capteur()
{
    // Led //
  adapter->update();
  bool on = ledOn.getValue().boolean;
  // active low led
  digitalWrite(ledPin, on ? LOW : HIGH); 
  if (on != lastOn) {
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

if (hc.dist() > 20){
    Serial.print("log: Value: ");
    Serial.println(hc.dist());

  //  lastValue = percent;
  delay(100);
  }
  else{
    Serial.print("log: Value: ");
    Serial.println(hc.dist());
    delay(100);
  }
}*/