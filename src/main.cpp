#include <Arduino.h>
#include "modbus.h"

// Pin numbers for LEDs
#define LED_GREEN 10
#define LED_YELLOW 11
#define LED_RED 12

// Pin numbers for buttons
#define BTN_GREEN 14
#define BTN_YELLOW 15
#define BTN_RED 16

// Modbus register for LED toggle
#define LED_REG 0

// Serial port info
#define SERIAL_PORT 0
#define BAUD_RATE 115200
#define TIMEOUT 50


// Define interrupts for each LED's corresponding button
void toggle_green() {
  digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
}

void toggle_yellow() {
  digitalWrite(LED_YELLOW, !digitalRead(LED_YELLOW));
}

void toggle_red() {
  digitalWrite(LED_RED, !digitalRead(LED_RED));
}

// Ran before the main loop
void setup() {
  // Set the LED pins as output pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Set the button pins as input pins as well as enable the pullup resistor
  // The PUR is enabled because otherwise the pin would be left floating
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_YELLOW, INPUT_PULLUP);
  pinMode(BTN_RED, INPUT_PULLUP);

  // Associate the defined interrupts with specific pins
  attachInterrupt(digitalPinToInterrupt(BTN_GREEN), toggle_green, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_YELLOW), toggle_yellow, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_RED), toggle_red, FALLING);

  // Initialize serial port and modbus
  portSetup(SERIAL_PORT, 0, BAUD_RATE, TIMEOUT);
  modbus_init(1);
}

// Poll modbus and toggle LED based on received data
void loop() {
  modbus_update();

  switch(charRegisters[LED_REG]) {
    case 'G':
      toggle_green();
      break;
    case 'Y':
      toggle_yellow();
      break;
    case 'R':
      toggle_red();
      break;
    default:
      break;
  }

  // Erase LED register so LED doesn't keep toggling
  charRegisters[LED_REG] = 0;
}
