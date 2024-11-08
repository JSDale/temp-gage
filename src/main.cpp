#include <Arduino.h>

// Thermistor parameters from the datasheet
#define RT0 100000
#define B 3950

// Our series resistor value = 10 kΩ
#define R 10000  

// Variables for calculations
float RT, VR, ln, TX, T0, VRT;

void setup() {
  // Setup serial communication
  Serial.begin(9600);
  // Convert T0 from Celsius to Kelvin
  T0 = 25 + 273.15;
}

void loop() {
  // Read the voltage across the thermistor
  VRT = (5.00 / 1023.00) * analogRead(A0);
  Serial.print("voltage on thermistor: ");
  Serial.println(VRT);
  
  // Calculate the voltage across the resistor
  VR = 5.00 - VRT;
  Serial.print("voltage on resistor: ");
  Serial.println(VR);

  // Calculate resistance of the thermistor
  RT = VRT / (VR / R);
  Serial.print("resistance of thermistor: ");
  Serial.println(RT);
  
  // Calculate temperature from thermistor resistance
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));

  // Convert to Celsius
  TX = TX - 273.15;
  
  Serial.print("Temperature: ");
  // Display in Celsius
  Serial.print(TX);                  
  Serial.print("C\t");
  
  delay(5000);
}