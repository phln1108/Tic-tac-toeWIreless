#include "espnow.h"

static esp_now_peer_info_t peerInfo;
// static uint8_t broadcastAddress[] = {0xc0, 0x49, 0xef, 0xe7, 0xc9, 0xF4}; // v4
static uint8_t broadcastAddress[] = {0x80, 0x7d, 0x3a, 0x81, 0x6e, 0x80}; // devkit
Callback* callback;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.print("mac: ");
  for( int i = 0; i< 6; i++) {
    Serial.printf("%2x:",mac[i]);                                     
  }
  Serial.print("\nBytes received: ");
  Serial.println(len);
  for( int i = 0; i< len; i++) {
    Serial.print(incomingData[i]);
  }
  Serial.println();
  callback(incomingData[0]);
}

void setupEspNow(Callback new_callback) {
    if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  callback = new_callback;
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

}

void sendData(uint8_t cell) {
  esp_now_send(broadcastAddress,&cell,1);
}
