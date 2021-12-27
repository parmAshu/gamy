#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println(WiFi.macAddress());
  delay(10000);
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:

}
