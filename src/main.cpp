#include <WiFi.h>

#include "espnow.h"
#include "tictactoe.h"

Tictactoe* game = nullptr;
uint8_t res = 0;
bool restarted = true;

void make_move(uint8_t* cell, uint8_t size) {
    res = game->play(cell[0], 2);
}

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());

    setupEspNow(make_move);
    startPairing();

    // game = new Tictactoe();
}

void loop() {
    if (esp_loop()) {
        if (game == nullptr)
            game = new Tictactoe;

        if (Serial.available()) {
            restarted = true;
            uint8_t cell = Serial.read() - '0';
            Serial.println(cell);
            res = game->play(cell, 1);
            
            if (res != 4 && res != 5)
                sendData(&cell, 1);
        }
        if (res != 0 && res != 4 && res !=5 && restarted) {
            game->restart();
            restarted = false;
        }
    }
}
