#include <Arduino.h>
#include <LiquidCrystal.h>

// Thermistor parameters from the datasheet
#define RT0 100000
#define B 3950

// Our series resistor value = 10 kΩ
#define R 10000

// Variables for calculations
float RT, VR, ln, TX, T0, VRT;

const int BuzzPin = 8;
const int LimitPin = 10;
int limitIndex = 0;
int limits[] = {50, 65, 85};

unsigned long previousM = millis();

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(BuzzPin, OUTPUT);
  pinMode(LimitPin, INPUT);
  // Convert T0 from Celsius to Kelvin
  T0 = 25 + 273.15;
  lcd.print("hello");
  delay(1000);
}

void PlayTone()
{
  auto m = millis();
  if ((m - previousM) > 1000)
  {
    tone(BuzzPin, 262);
  }
  else
  {
    tone(BuzzPin, 349);
  }
  previousM = m;
}

void StopTone()
{
  noTone(BuzzPin);
}

void loop() {
  if (digitalRead(LimitPin) == HIGH)
  {
    limitIndex++;
    if (limitIndex > 3) limitIndex = 0;
  }

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
  lcd.clear();
  lcd.print(String(TX) + " C");
  lcd.setCursor(0, 2);
  lcd.print("Warning at: " + String(limits[limitIndex]));

  if (TX > limits[limitIndex])
  {
    PlayTone();
  }
  else
  {
    StopTone();
  }
  
  delay(2000);
}