#include <Arduino.h>

const int thermistorPin = A0;     // Analog pin used to connect the thermistor
const int seriesResistor = 10000; // Resistance of the series resistor in Ohms
const float nominalResistance = 10000; // Resistance at room temperature in Ohms
const float tempNominal = 25;      // Room temperature in Celsius
const float betaCoefficient = 3950; // Thermistor beta coefficient

void setup() 
{
  Serial.begin(9600);
  pinMode(thermistorPin, INPUT);
}

float CalculateTemperature(float resistance) 
{
  float lnRt = log(resistance / nominalResistance);
  Serial.print("resistance: ");
  Serial.println(lnRt);
  float temperature = 1 / (betaCoefficient * lnRt + 1 / (tempNominal + 273.15));
  Serial.print("Kelvin: ");
  Serial.println(temperature);
  return temperature - 273.15; // Convert Kelvin to Celsius
}

float MyMap(float reading, int min, int max, int outMin, int outMax)
{
  auto inRange = max - min;
  auto percentage = (reading / inRange) * 100;
  auto outRange = outMax - outMin;
  auto value = (percentage / 100) * outRange;
  return value;
}

void loop() 
{
  int rawReading = analogRead(thermistorPin);
  Serial.print("raw reading: ");
  Serial.println(rawReading);
  int resistance = MyMap(rawReading, 0, 1023, 0, seriesResistor);
  float temperature = CalculateTemperature(resistance);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  delay(5000);
}
