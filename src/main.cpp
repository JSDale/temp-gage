#include <Arduino.h>
#include <LiquidCrystal.h>

// Thermistor parameters from the datasheet
#define RT0 100000
#define B 3950

// Our series resistor value = 10 kΩ
#define R 10000

// Variables for calculations
float RT, VR, ln, TX, T0, VRT;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  // Convert T0 from Celsius to Kelvin
  T0 = 25 + 273.15;
  lcd.print("hello");
  delay(1000);
}

void loop() {
  lcd.clear();
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
  lcd.print(String(TX) + " C");
  
  delay(2000);
}