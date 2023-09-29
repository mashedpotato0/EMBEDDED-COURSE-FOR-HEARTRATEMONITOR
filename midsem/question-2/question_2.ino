/*
Aim: Demonstrate a breadboard level, wireless wrist pulse sensing & recording platform as discussed
in the class.
[12]
1. The system must simultaneously plot Vaata (V), Pitta (P), and Kapha (K) waveforms.
2. Further, there should be a recording/logging facility to save these three V,P,K waveforms as required.
*/

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

#define TCAADDR 0x70
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  Wire.begin();
  delay(0);

  tcaselect(2);
  if (!sensor1.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  sensor1.setup();
  sensor1.setPulseAmplitudeRed(0);
  sensor1.setPulseAmplitudeGreen(0x0A);


  tcaselect(3);
  if (!sensor2.begin(Wire, I2C_SPEED_FAST))
  {
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
  if (!sensor3.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  sensor3.setup();
  sensor3.setPulseAmplitudeRed(0);
  sensor3.setPulseAmplitudeGreen(0x0A);
}

void loop()
{
  tcaselect(2);
  long irValue1 = sensor1.getIR();
  Serial.print(irValue1);
  Serial.print(",");

  tcaselect(3);
  long irValue2 = sensor2.getIR();
  Serial.print(irValue2);
  Serial.print(",");

  tcaselect(4);
  long irValue3 = sensor3.getIR();
  Serial.println(irValue3);

  delay(10);
}
