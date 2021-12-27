#include <ESP8266WiFi.h>
#include <espnow.h>

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {

  for( uint8_t i=0; i<6; i++)
  {
    Serial.print( mac[i], HEX );
  }
  Serial.println();
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println( (char)(*incomingData) );
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
