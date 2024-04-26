#pragma once

#include <esp_now.h>
#include <Arduino.h>

typedef void (Callback)(uint8_t cell);

void setupEspNow(Callback callback);
void sendData(uint8_t cell);



