#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 sensor1;
MAX30105 sensor2;
MAX30105 sensor3;
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;
int perCent1;
int irOffset1 = 2100;
const int analogOutPin1 = 27;
const int analogOutPin2 = 26;
const int analogOutPin3 = 25;

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

const float lowPassAlpha = 0.1;
const float highPassAlpha = 0.5;
float prevIrValue1Filtered = 0;
float prevIrValue2Filtered = 0;
float prevIrValue3Filtered = 0;
float prevIrValue1 = 0;
float prevIrValue2 = 0;
float prevIrValue3 = 0;

float lowPassFilter(float input, float prevOutput, float alpha) {
  return alpha * input + (1 - alpha) * prevOutput;
}

float highPassFilter(float input, float prevInput, float prevOutput, float alpha) {
  return alpha * (prevOutput + input - prevInput);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
  Wire.begin();
  delay(0);

  tcaselect(2);
  if (!sensor1.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  sensor1.setup();
  sensor1.setPulseAmplitudeRed(0);
  sensor1.setPulseAmplitudeGreen(0x0A);

  tcaselect(3);
  if (!sensor2.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  byte ledBrightness2 = 25;
  byte sampleAverage2 = 4;
  byte ledMode2 = 2;
  int sampleRate2 = 400;
  int pulseWidth2 = 411;
  int adcRange2 = 2048;
  sensor2.setup(ledBrightness2, sampleAverage2, ledMode2, sampleRate2, pulseWidth2, adcRange2);
  sensor2.setPulseAmplitudeRed(0);
  sensor2.setPulseAmplitudeGreen(0x0A);

  tcaselect(4);
  if (!sensor3.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  sensor3.setup();
  sensor3.setPulseAmplitudeRed(0);
  sensor3.setPulseAmplitudeGreen(0x0A);
}

void loop() {
  tcaselect(2);
  long irValue1 = sensor1.getIR();
  float irValue1Filtered = lowPassFilter(irValue1, prevIrValue1Filtered, lowPassAlpha);
  float irValue1BandPass = highPassFilter(irValue1Filtered, prevIrValue1, prevIrValue1Filtered, highPassAlpha);
  prevIrValue1 = irValue1Filtered;

  tcaselect(3);
  long irValue2 = sensor2.getIR();
  float irValue2Filtered = lowPassFilter(irValue2, prevIrValue2Filtered, lowPassAlpha);
  float irValue2BandPass = highPassFilter(irValue2Filtered, prevIrValue2, prevIrValue2Filtered, highPassAlpha);
  prevIrValue2 = irValue2Filtered;

  tcaselect(4);
  long irValue3 = sensor3.getIR();
  float irValue3Filtered = lowPassFilter(irValue3, prevIrValue3Filtered, lowPassAlpha);
  float irValue3BandPass = highPassFilter(irValue3Filtered, prevIrValue3, prevIrValue3Filtered, highPassAlpha);
  prevIrValue3 = irValue3Filtered;

  Serial.print(-irValue1BandPass);
  Serial.print(",");
  Serial.print(-irValue2BandPass);
  Serial.print(",");
  Serial.println(-irValue3BandPass);

  delay(20);
}
