#include "espnow.h"

#define PARING_TIME 5000

Callback *callback;

static uint8_t deviceMac[6];
static PairingState pairingState = NOT_PAIRIED;
uint8_t buffer[sizeof(Msg)];

//se time
static unsigned long paringTime = 0;
static unsigned long connectionTime = 0;
static bool pinged = false;

bool isSameMac(uint8_t *mac1, uint8_t *mac2) {
    for (uint8_t i = 0; i < 6; i++) {
        if (mac1[i] != mac2[i])
            return false;
    }
    return true;
}

void printData(uint8_t *mac, Msg msg, bool send = false) {
    Serial.printf("Data %s: ", send ? "sended to" : "received from");
    for (uint8_t i = 0; i < 6; i++) {
        Serial.printf("%X%c", mac[i], i == 5 ? '\n' : ':');
    }

    Serial.print("Type of message: ");
    String type = "";
    switch (msg.typeOfMsg) {
        case PAIR:
            type = "PAIR";
            break;
        case CONFIRM_PAIRING:
            type = "CONFIRM_PAIRING";
            break;
        case NORMAL:
            type = "NORMAL";
            break;
        case PAIRING_ACCEPT:
            type = "PAIRING_ACCEPT";
            break;
        case PING:
            type = "PING";
            break;
        case RETURN_OK:
            type = "RETURN_OK";
            break;
    }
    Serial.println(type);
    Serial.println(pairingState);
    Serial.print("Data: ");
    for (uint8_t i = 0; i < msg.size; i++) {
        Serial.printf("%X%c", mac[i], i == msg.size - 1 ? '\n' : ' ');
    }
}

void sendMsg(uint8_t *mac, Msg msg) {
    // printData(mac, msg, true);

    memcpy(buffer, &msg, sizeof(Msg));

    esp_now_send(BROADCAST_MAC, buffer, sizeof(Msg));
}

void sendDataPairing(uint8_t *mac, TypeOfMsg type = PAIR) {
    Msg msg;
    msg.typeOfMsg = type;
    msg.data[0] = 1;
    msg.size = 1;

    sendMsg(mac, msg);
}

void sendData(uint8_t *data, uint8_t size) {
    if (pairingState != PAIRED)
        return;

    Msg msg;
    memcpy(msg.data, data, sizeof(size));
    msg.size = size;
    msg.typeOfMsg = NORMAL;

    sendMsg(deviceMac, msg);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Serial.print("Sended Packet Status: ");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
    Msg msg;
    memcpy(&msg, data, sizeof(Msg));

    // printData((uint8_t *)mac, msg);

    if (msg.typeOfMsg == PAIR && pairingState == PAIRING) {
        memcpy(deviceMac, mac, 6);

        sendDataPairing(BROADCAST_MAC,CONFIRM_PAIRING);
        return;
    }

    if (msg.typeOfMsg == CONFIRM_PAIRING && pairingState == PAIRING) {
        memcpy(deviceMac, mac, 6);
        pairingState = CONFIRMING_PAIRING;

        sendDataPairing(BROADCAST_MAC,CONFIRM_PAIRING);
        return;
    }

    if (!isSameMac((uint8_t *)mac, deviceMac))
        return;

    pinged = true;
    connectionTime = millis();

    if (msg.typeOfMsg == CONFIRM_PAIRING && pairingState == CONFIRMING_PAIRING) {
        pairingState = PAIRED;

        sendDataPairing(deviceMac,PAIRING_ACCEPT);
        return;
    }

    if (msg.typeOfMsg == PAIRING_ACCEPT && pairingState == CONFIRMING_PAIRING) {
        pairingState = PAIRED;

        sendDataPairing(deviceMac,PAIRING_ACCEPT);
        return;
    }

    if (msg.typeOfMsg == PAIRING_ACCEPT && pairingState == PAIRED || msg.typeOfMsg == PING) {
        sendDataPairing(deviceMac,RETURN_OK);
        return;
    }

    if (msg.typeOfMsg == NORMAL && pairingState == PAIRED) {
        callback(msg.data, msg.size);
    }
}

void setupEspNow(Callback new_callback) {
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    callback = new_callback;

    esp_now_peer_info_t broadcastPeer;
    memset(&broadcastPeer, 0, sizeof(broadcastPeer));
    memcpy(broadcastPeer.peer_addr, BROADCAST_MAC, sizeof(BROADCAST_MAC));
    broadcastPeer.channel = 0;
    broadcastPeer.encrypt = false;
    esp_now_add_peer(&broadcastPeer);

    esp_now_register_send_cb(OnDataSent);

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
}

bool esp_loop() {
    if (pairingState == PAIRING && millis() - paringTime > PARING_TIME) {
        Serial.println("pairing");
        sendDataPairing(BROADCAST_MAC,PAIR);
        paringTime = millis();
        return false;
    }

    if (pairingState == PAIRED && millis() - connectionTime > PARING_TIME) {
        connectionTime = millis();

        if (!pinged) {
            Serial.println("nao pingou");
            pairingState = PAIRING;
            return false;
        }

        if (pairingState == PAIRED)
            sendDataPairing(deviceMac,PING);
        pinged = false;
    }

    if (pairingState == PAIRED)
        return true;

    return false;
}

void startPairing() {
    if (pairingState != PAIRING)
        pairingState = PAIRING;
}
