#include <esp_now.h>
#include <WiFi.h>

#define X_JOYSTICK_PIN 32
#define Y_JOYSTICK_PIN 33

uint8_t broadcastAddress[] = {0xB8, 0xD6, 0x1A, 0xA7, 0x40, 0x24};
const int joystickEeadZone = 127;

typedef struct data_packet {
  byte x;
  byte y;
} data_packet;

data_packet payload;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

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
}

byte mapJoysticToPWM(int value){
  if (value >= 2200){
    value = map(value, 2200, 4095, joystickEeadZone, 254);
  }
  else if (value <= 1800){
    value = map(value, 1800, 0, joystickEeadZone, 0);  
  }
  else{
    value = joystickEeadZone;
  }
  return value;
}
 
void loop() {
  // Set values to send
  payload.x = mapJoysticToPWM(analogRead(X_JOYSTICK_PIN));
  payload.y = mapJoysticToPWM(analogRead(Y_JOYSTICK_PIN));
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(payload));
  delay(1);
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}
