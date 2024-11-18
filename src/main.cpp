#include <Arduino.h>
#include <LiquidCrystal.h>

// Thermistor parameters from the datasheet
#define RT0 100000
#define B 3950

// Our series resistor value = 10 kΩ
#define R 10000

// Variables for calculations
float thermistorResistance, voltageAcrossResistor, ln, temperature, T0, voltageAcrossThermistor;

const int BuzzPin = 8;
const int LimitChangePin = 10;
int limitIndex = 0;
int limits[] = {50, 65, 85};
const int ButtonDepressDebounceLimit = 28000;
unsigned long buttonPresses = 0;
unsigned long lastPress = millis();
unsigned int pressWait = 800;

unsigned long previousTone = millis();
unsigned long lastPrinted = millis();
const unsigned int PrintDelay = 1000;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(BuzzPin, OUTPUT);
  pinMode(LimitChangePin, INPUT_PULLUP);
  // Convert T0 from Celsius to Kelvin
  T0 = 25 + 273.15;
  lcd.print("hello");
  delay(1000);
}

void PlayTone()
{
  auto m = millis();
  if ((m - previousTone) > 1000)
  {
    tone(BuzzPin, 262);
  }
  else
  {
    tone(BuzzPin, 349);
  }
  previousTone = m;
}

void StopTone()
{
  noTone(BuzzPin);
}

void PrintTemperature()
{
  Serial.print("Temperature: ");
  // Display in Celsius
  Serial.print(temperature);                  
  Serial.print("C\t");
  lcd.clear();
  lcd.print(String(temperature) + " C");
  lcd.setCursor(0, 2);
  lcd.print("Warning at: " + String(limits[limitIndex]));
}

void DetectButtonPress()
{
  if (lastPress > millis() + pressWait) buttonPresses = 0;
  if (digitalRead(LimitChangePin) == LOW)
  {
    buttonPresses++;
  }
  
  if (buttonPresses > ButtonDepressDebounceLimit)
  {
    limitIndex++;
    if (limitIndex > 3) limitIndex = 0;
    buttonPresses = 0;
  }
}

void CalculateTemperature()
{
  // Read the voltage across the thermistor
  voltageAcrossThermistor = (5.00 / 1023.00) * analogRead(A0);
  //Serial.print("voltage on thermistor: ");
  //Serial.println(voltageAcrossThermistor);
  
  // Calculate the voltage across the resistor
  voltageAcrossResistor = 5.00 - voltageAcrossThermistor;
  //Serial.print("voltage on resistor: ");
  //Serial.println(voltageAcrossResistor);

  // Calculate resistance of the thermistor
  thermistorResistance = voltageAcrossThermistor / (voltageAcrossResistor / R);
  //Serial.print("resistance of thermistor: ");
  //Serial.println(thermistorResistance);
  
  // Calculate temperature from thermistor resistance
  ln = log(thermistorResistance / RT0);
  temperature = (1 / ((ln / B) + (1 / T0)));

  // Convert to Celsius
  temperature = temperature - 273.15;
}

void loop() 
{
  DetectButtonPress();
  CalculateTemperature();
  if (lastPrinted > PrintDelay + millis())
  {
    PrintTemperature();
    lastPrinted = millis();
  }

  if (temperature > limits[limitIndex])
  {
    PlayTone();
  }
  else
  {
    StopTone();
  }
}