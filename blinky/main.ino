#include <EEPROM.h>

// Add EEPROM address constant for storing the mode
const int eepromAddress = 0;  // Use address 0 for storing the mode

const int switchPin = 2;
const int blueLed = 3;
const int redLed1 = 4;
const int redLed2 = 5;
volatile int mode = 0;
unsigned long lastToggleTime = 0;
const long interval = 1000;  // 1 seconds

volatile bool shouldBlink = false;
volatile int blinkCount = 0;
const int blinkInterval = 100;  // 100ms for a fast blink

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(switchPin), changeMode, FALLING);
  mode = EEPROM.read(eepromAddress) % 3;
}

void loop() {
  unsigned long currentMillis = millis();

  if (shouldBlink) {
    blinkLed();  // Call a modified blinkLed function
  } else {
    toggleLeds();  // Continue with normal LED operation
  }
}

void changeMode() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 250) {  // Debounce
    mode = (mode + 1) % 3;
    EEPROM.write(eepromAddress, mode); // Remember mode between reboots

    // turn off all LEDs
    digitalWrite(blueLed, LOW);
    digitalWrite(redLed1, LOW);
    digitalWrite(redLed2, LOW);

    shouldBlink = true;
    blinkCount = 0;
  }

  lastInterruptTime = interruptTime;
}

void blinkLed() {
  static unsigned long lastBlinkTime = 0;
  int pin = (mode == 0) ? blueLed : (mode == 1) ? redLed1
                                                : redLed2;

  if (millis() - lastBlinkTime >= blinkInterval && blinkCount < 8) {  // Blink 4 times (on and off)
    digitalWrite(pin, !digitalRead(pin));
    lastBlinkTime = millis();
    blinkCount++;
    if (blinkCount >= 8) {
      shouldBlink = false;
      digitalWrite(pin, LOW);     // Ensure LED is off after blinking
      lastToggleTime = millis();  // Ensure we update last toggle time so light is off starting cycle
    }
  }
}


void toggleLeds() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastToggleTime >= interval) {
    if (mode == 0) {
      // Toggle blue LED every 10 seconds
      digitalWrite(blueLed, !digitalRead(blueLed));
    } else if (mode == 1) {
      // Toggle first red LED every 10 seconds
      digitalWrite(redLed1, !digitalRead(redLed1));
    } else if (mode == 2) {
      // Toggle second red LED every 10 seconds
      digitalWrite(redLed2, !digitalRead(redLed2));
    }
    lastToggleTime = currentMillis;
  }
}
