#include <ESP8266WiFi.h>
#include <espnow.h>

typedef enum gamy_team{ RED_TEAM = 1, BLUE_TEAM = 2, GREEN_TEAM = 3, YELLOW_TEAM = 4, INVALID_TEAM = 0 }gamy_team;

typedef struct gamy_message{
  gamy_team team;
}gamy_message;

gamy_message msg; 

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  msg.team = RED_TEAM;

}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg) );

    lastTime = millis();
  }
}
