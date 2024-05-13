// // #include <WiFi.h>

// // #include "espnow.h"
// // #include "tictactoe.h"

// // Tictactoe* game;
// // uint8_t res = 0;
// // bool restarted = true;

// // void make_move(uint8_t cell) {
// //   res = game->play(cell, 2);
// // }

// // void setup() {
// //     // Init Serial Monitor
// //     Serial.begin(115200);

// //     // Set device as a Wi-Fi Station
// //     WiFi.mode(WIFI_STA);
// //     Serial.println(WiFi.macAddress());

// //     setupEspNow(make_move);

// //     game = new Tictactoe();
// // }

// // void loop() {
// //     if (Serial.available()) {
// //         restarted = true;
// //         uint8_t cell = Serial.read() - '0';
// //         Serial.println(cell);
// //         res = game->play(cell, 1);
// //         if (res != 4) {
// //             sendData(cell);
// //         }
// //     }
// //     if (res != 0 && res != 4 && restarted) {
// //         game->restart();
// //         restarted = false;
// //     }
// //     delay(100);
// // }

// #include <WiFi.h>
// #include <esp_now.h>

// #define MAX_DEVICES 5
// #define DATA_SIZE 20
// uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// struct PairedDevice {
//     uint8_t macAddress[6];
//     bool active;
// };

// PairedDevice pairedDevices[MAX_DEVICES];
// int numPairedDevices = 0;

// void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int len) {
//     // Process received data here
//     Serial.print("Received data from ");
//     for (int i = 0; i < 6; i++) {
//         Serial.print(macAddr[i], HEX);
//         if (i < 5) Serial.print(":");
//     }
//     Serial.print(": ");
//     Serial.write(data, len);
//     Serial.println();

//     // Handle discovery phase
//     if (len == 0 && memcmp(macAddr, (void *)BROADCAST_MAC, sizeof(BROADCAST_MAC)) == 0) {
//         // Received a broadcast packet
//         // Add the sender to the list of paired devices
//         if (numPairedDevices < MAX_DEVICES) {
//             memcpy(pairedDevices[numPairedDevices].macAddress, macAddr, 6);
//             pairedDevices[numPairedDevices].active = true;
//             numPairedDevices++;
//             Serial.println("Paired with a device");
//         }
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     WiFi.mode(WIFI_STA);

//     if (esp_now_init() != ESP_OK) {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }

//     esp_now_register_recv_cb(onDataReceived);

//     // Register broadcast address for receiving discovery packets
//     esp_now_peer_info_t broadcastPeer;
//     memset(&broadcastPeer, 0, sizeof(broadcastPeer));
//     memcpy(broadcastPeer.peer_addr, BROADCAST_MAC, sizeof(BROADCAST_MAC));
//     broadcastPeer.channel = 0;
//     broadcastPeer.encrypt = false;
//     esp_now_add_peer(&broadcastPeer);
// }

// void loop() {
//     // Data exchange
//     if (numPairedDevices > 0) {
//         uint8_t dataToSend[DATA_SIZE] = "Hello, ESP-NOW!";
//         for (int i = 0; i < numPairedDevices; i++) {
//             if (pairedDevices[i].active) {
//                 esp_now_send(pairedDevices[i].macAddress, dataToSend, sizeof(dataToSend));
//             }
//         }
//         Serial.println("Data sent to paired devices");
//     } else {
//         Serial.println("sending data to discover devices");
//         const uint8_t opa = 1;
//         esp_now_send(BROADCAST_MAC,&opa,1);
//     }

//     delay(5000);  // Send data every 5 seconds
// }
#include <WiFi.h>

#include "espnow.h"
#include "tictactoe.h"

Tictactoe* game;
uint8_t res = 0;
bool restarted = true;

void make_move(uint8_t* cell, uint8_t size) {
    res = game->play(cell[0], 2);
}

void setup() {
    // Init Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());

    setupEspNow(make_move);
    startPairing();

    game = new Tictactoe();
}

void loop() {
    if (esp_loop()) {
        if (Serial.available()) {
            restarted = true;
            uint8_t cell = Serial.read() - '0';
            Serial.println(cell);
            res = game->play(cell, 1);
            if (res != 4) {
                sendData(&cell, 1);
            }
        }
        if (res != 0 && res != 4 && restarted) {
            game->restart();
            restarted = false;
        }
    }
}
