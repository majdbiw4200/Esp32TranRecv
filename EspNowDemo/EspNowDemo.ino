#include <esp_now.h>
#include <WiFi.h>

// Replace with your Esp MAC address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup() {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Set up the LED
  pinMode(33, OUTPUT);

  // Register the callback function to receive messages
  esp_now_register_recv_cb([](const uint8_t *mac, const uint8_t *data, int len) {
    Serial.print("Received message: ");
    Serial.println(*(bool*)data);
    digitalWrite(33, *(bool*)data);
});
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '1') {
      // Send a message to the receiving board to turn off the LED
      bool message = true;
      esp_now_send(broadcastAddress, (uint8_t*)"true", sizeof("true"));
    } else if (c == '0') {
      // Send a message to the receiving board to turn on the LED
      bool message = false;
      esp_now_send(broadcastAddress, (uint8_t*)&message, sizeof(bool));
    }
  }
}



