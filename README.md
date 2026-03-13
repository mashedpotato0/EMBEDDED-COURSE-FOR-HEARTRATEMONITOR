# Embedded Systems Course — Heart Rate Monitor

Mid-semester coursework for an embedded systems course, implementing a heart rate monitoring system on microcontrollers (Arduino / ESP32).

## Structure

```
EMBEDDED-COURSE-FOR-HEARTRATEMONITOR/
└── midsem/
    ├── question-1/
    │   └── question1midsem.ino      # Arduino sketch for Question 1
    └── question-2/
        ├── question_2.ino           # Main Arduino sketch for Question 2
        ├── question2bandpass/
        │   └── question2bandpass.ino # Band-pass filter implementation
        └── esp32_gui/
            ├── esp32_gui.pde        # Processing GUI for ESP32 serial data
            └── readme               # Notes for the ESP32 GUI
```

## Contents

### Question 1
An Arduino sketch implementing heart-rate-related embedded logic for the mid-semester exam.

### Question 2
A more complex implementation consisting of:
- **Main sketch** (`question_2.ino`) — Core heart rate monitoring logic on ESP32/Arduino.
- **Band-pass filter** (`question2bandpass.ino`) — Signal conditioning / noise filtering for sensor input.
- **Processing GUI** (`esp32_gui.pde`) — A real-time desktop visualization tool built with [Processing](https://processing.org/) that reads serial data from the ESP32 and displays it graphically.

## Requirements

- **Arduino / ESP32** microcontroller
- [Arduino IDE](https://www.arduino.cc/en/software) (to flash `.ino` sketches)
- [Processing](https://processing.org/download/) (to run the serial GUI)
