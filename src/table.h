#pragma once
#include <Arduino.h>

enum LedState: uint8_t {
    LED_OFF,
    LED_ON,
    LED_BLINK
};

const uint8_t minusLed[3] = {18,19,21};
const uint8_t plusLed[3] = {32,33,25};

void handleLed();

uint8_t loopTable();

void setupTable();

void setLedState(uint8_t led,LedState state);