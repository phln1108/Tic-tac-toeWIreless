#include "table.h"

// #include <Arduino.h>

#define DATA 13
#define CLOCK 26
#define BLINK_TIME 500

long lastBlink = 0;

const uint8_t rowsPin[3] = {12, 27, 0};

static bool blinkState = false;

static LedState leds[9] = {LED_OFF};

void wait(long time) {
    long markedTime = millis();
    while (millis() - markedTime <= time) {
    }
}

void setupTable() {
    pinMode(DATA, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(4, OUTPUT);

    for (uint8_t i = 0; i < 3; i++) {
        pinMode(rowsPin[i], INPUT_PULLUP);
        pinMode(minusLed[i], OUTPUT);
        pinMode(plusLed[i], OUTPUT);
    }
}

uint8_t loopTable() {
    digitalWrite(DATA, true);
    wait(1);
    uint8_t pin = 10; 
    for (uint8_t i = 0; i < 3; i++) {
        digitalWrite(CLOCK, true);
        digitalWrite(DATA, false);

        for (uint8_t j = 0; j < 3; j++) {
            if (!digitalRead(rowsPin[j]) && pin == 10) 
                pin = j + i * 3;
        }

        digitalWrite(CLOCK, false);
        wait(1);
    }

    digitalWrite(CLOCK, true);
    wait(1);
    digitalWrite(CLOCK, false);

    return 10;
}

void handleLed() {
    for (uint8_t i = 0; i < 9; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            digitalWrite(plusLed[j], false);
            digitalWrite(minusLed[j], true);
        }

        if (leds[i] == LED_BLINK) {
            if (millis() - lastBlink > BLINK_TIME) {
                blinkState = !blinkState;
                lastBlink = millis();
            }
            digitalWrite(plusLed[i % 3], blinkState);
            digitalWrite(minusLed[(i - (i % 3)) / 3], !blinkState);
            wait(1);

            continue;
        }

        bool ison = leds[i] == LED_ON;

        digitalWrite(plusLed[i % 3], ison);
        digitalWrite(minusLed[(i - (i % 3)) / 3], !ison);
        wait(1);
    }
}

void setLedState(uint8_t led, LedState state) {
    if (led > 8)
        return;
    leds[led] = state;
}