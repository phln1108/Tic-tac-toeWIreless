#include <WiFi.h>

#include "espnow.h"
#include "table.h"
#include "tictactoe.h"

Tictactoe* game = nullptr;
uint8_t res = 0;
bool restarted = true;

void make_move(uint8_t* cell, uint8_t size) {
    res = game->play(cell[0], 2);
}

void setup() {
    Serial.begin(115200);
    Serial.println("aaaa");
    setupTable();
    Serial.println("aaaa");

    setLedState(1,LED_ON);
    setLedState(4,LED_ON);
    setLedState(7,LED_ON);
    setLedState(0,LED_BLINK);
    setLedState(8,LED_BLINK);


    // WiFi.mode(WIFI_STA);
    // Serial.println(WiFi.macAddress());

    // setupEspNow(make_move);
    // startPairing();
}

void loop() {
    uint8_t value = loopTable();
    if (value != 10)
        digitalWrite(4, true);

    Serial.println(value);
    

    handleLed();
    // if (esp_loop()) {
    //     if (game == nullptr)
    //         game = new Tictactoe;

    //     if (Serial.available()) {
    //         restarted = true;
    //         uint8_t cell = Serial.read() - '0';
    //         Serial.println(cell);
    //         res = game->play(cell, 1);

    //         if (res != 4 && res != 5)
    //             sendData(&cell, 1);
    //     }
    //     if (res != 0 && res != 4 && res !=5 && restarted) {
    //         game->restart();
    //         restarted = false;
    //     }
    // }
}
