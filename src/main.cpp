#include <Arduino.h>

const int thermistorPin = A0;     // Analog pin used to connect the thermistor

void setup() 
{
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
}

void loop() 
{
  float input = analogRead(thermistorPin);
  float voltage = (input/1024.0) * 5.0;
  Serial.print("Voltage: ");
  Serial.println(voltage);
  float temp = (voltage - 0.5) * 100;
  Serial.print("Temp C: ");
  Serial.println(temp);
  delay(5000);
}
