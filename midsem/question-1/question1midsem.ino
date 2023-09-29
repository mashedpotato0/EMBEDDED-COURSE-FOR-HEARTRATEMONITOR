/*
Aim:
Using ESP32/8051 platform, design and demonstrate a function generator system equipped with four buttons/touch/tap pins to control the system in real-time as follows.
1. Button 1 would facilitate to switch between different type of waveform at certain frequency: Square, Sinusoid, Sawtooth, Triangular.
2. Button 2 would facilitate to scale the amplitude of the displayed waveform [0.5x,1x,2x]
3. Button 3 would scale the frequency of the chosen waveform [0.5x,1x,2x].
4. Button 4 would facilitate to scale DAC resolution of the chosen waveform [3 bit,5 bit,8 bit]
*/

#include <Arduino.h>
#include <SPI.h>

const int button1Pin = 27;
const int button2Pin = 26;
const int button3Pin = 25;
const int button4Pin = 33;
int i = 0;
const int dacChipSelect = 5;
const int analogOutPin = 25;
int refresh = 64;
int waveform = 0;
int frequencyMultiplier = 1;
int amplitudeMultiplier = 1;
int dacResolution = 3;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(dacChipSelect, OUTPUT);
  digitalWrite(dacChipSelect, HIGH);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(button1Pin) == LOW) {
    waveform = (waveform + 1) % 4;
    delay(200);
  }

  if (digitalRead(button2Pin) == LOW) {
    frequencyMultiplier = (frequencyMultiplier == 2) ? 4 : ((frequencyMultiplier == 8) ? 8 : 2);
    delay(200);
  }

  if (digitalRead(button3Pin) == LOW) {
    amplitudeMultiplier = (amplitudeMultiplier == 4) ? 4 : ((amplitudeMultiplier == 8) ? 8 : 2);
    if (amplitudeMultiplier == 0)
      amplitudeMultiplier = 1;
    delay(200);
  }

  if (digitalRead(button4Pin) == LOW) {
    dacResolution = (dacResolution == 3) ? 5 : ((dacResolution == 5) ? 8 : 3);
    delay(200);
  }

  generateWaveform(i);
  i = (i + 1) % (refresh / frequencyMultiplier);
}

void generateWaveform(int i) {
  int amplitude = 100 * amplitudeMultiplier;
  int waveValue = 0;
  int maxDacValue = pow(2, dacResolution) - 1;

  switch (waveform) {
    case 0:
      waveValue = float((amplitude / 2.0) * sin(2 * PI * i / (refresh / frequencyMultiplier)) + (amplitude / 2.0));
      break;

    case 1:
      if (i < int(0.5 * refresh / frequencyMultiplier)) {
        waveValue = int((amplitude / 16) * i * 2);
      } else {
        waveValue = int((amplitude / 16) * (refresh / frequencyMultiplier - i) * 2);
      }
      break;

    case 2:
      waveValue = int((amplitude / (refresh / frequencyMultiplier)) * i);
      break;

    case 3:
      if (i < int(0.5 * refresh / frequencyMultiplier)) {
        waveValue = (amplitude / 2);
      } else {
        waveValue = 0;
      }
      break;
  }
  switch (dacResolution) {
    case 3:
      waveValue = map(waveValue, 0, 7, 0, 255);
      break;
    case 5:
      waveValue = map(waveValue, 0, 31, 0, 255);
      break;
    case 8:
      waveValue = map(waveValue, 0, 255, 0, 255);
      break;
  }

  Serial.println(waveValue);
}
