#include <WiFi.h>

#include "espnow.h"
#include "tictactoe.h"

Tictactoe* game;
uint8_t res = 0;
bool restarted = true; 

void make_move(uint8_t cell) {
  res = game->play(cell, 2);
}

void setup() {
    // Init Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());

    setupEspNow(make_move);

    game = new Tictactoe();
}

void loop() {
    if (Serial.available()) {
        restarted = true;
        uint8_t cell = Serial.read() - '0';
        Serial.println(cell);
        res = game->play(cell, 1);
        if (res != 4) {
            sendData(cell);
        }
    }
    if (res != 0 && res != 4 && restarted) {
        game->restart();
        restarted = false;
    }
    delay(100);
}