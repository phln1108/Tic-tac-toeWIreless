#pragma once

#include <Arduino.h>
#include <esp_now.h>

static uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

enum TypeOfMsg : uint8_t {
    NORMAL,
    PAIR,
    CONFIRM_PAIRING,
    PAIRING_ACCEPT,
    PAIRING_DECLINE,
    PAIRING_END,
    PING,
    RETURN_OK,
};

enum PairingState : uint8_t {
    NOT_PAIRIED,
    PAIRING,
    REQUESTING_PAIR,
    CONFIRMING_PAIRING,
    PAIRED,
};

struct Msg {
    TypeOfMsg typeOfMsg;
    uint8_t data[220];
    uint8_t size;
};

typedef void(Callback)(uint8_t *data,uint8_t size);

void setupEspNow(Callback callback);
void sendData(uint8_t *data, uint8_t size);
bool esp_loop();
void startPairing();
