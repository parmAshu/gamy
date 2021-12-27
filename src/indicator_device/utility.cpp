/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : utility.cpp
 * @brief : This file contains some utility functions and classes.
*/
#include "utility.h"

static uint8_t masterMacAddress[] = MASTER_MAC_ADDRESS;

static gamy_team buff[BUFF_LENGTH];
static uint8_t filled;
static uint8_t inPos=0;
static uint8_t outPos=0;

void enqueue( gamy_team team )
{
  if( filled < BUFF_LENGTH )
  {
    buff[inPos++] = team;
    inPos = ( inPos == BUFF_LENGTH ) ? 0 : inPos;
    filled++;
  }
}

gamy_team dequeue()
{
  gamy_team team = INVALID_TEAM;
  if( filled )
  {
    team = buff[outPos++];
    outPos = ( outPos == BUFF_LENGTH ) ? 0 : outPos;
    filled--;
  }
  return team;
}

void empty()
{
  inPos = outPos = filled = 0;
}

void InitGeneral()
{
  Serial.begin( SERIAL_BAUDRATE );

  pinMode( LED_GPIO, OUTPUT );
  pinMode( BUZZER_GPIO, OUTPUT );
  pinMode( RED_TEAM_GPIO, OUTPUT );
  pinMode( BLUE_TEAM_GPIO, OUTPUT );
  pinMode( GREEN_TEAM_GPIO, OUTPUT );
  pinMode( YELLOW_TEAM_GPIO, OUTPUT );

  for( uint8_t i=1; i<=5; i++){

  digitalWrite( LED_GPIO, LED_ON_LEVEL );
  digitalWrite( BUZZER_GPIO, LED_ON_LEVEL );
  digitalWrite( RED_TEAM_GPIO, RELAY_ON_LEVEL );
  digitalWrite( BLUE_TEAM_GPIO, RELAY_ON_LEVEL );
  digitalWrite( GREEN_TEAM_GPIO, RELAY_ON_LEVEL );
  digitalWrite( YELLOW_TEAM_GPIO, RELAY_ON_LEVEL );

  delay(1000);

  digitalWrite( LED_GPIO, LED_OFF_LEVEL );
  digitalWrite( BUZZER_GPIO, LED_OFF_LEVEL );
  digitalWrite( RED_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( BLUE_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( GREEN_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( YELLOW_TEAM_GPIO, RELAY_OFF_LEVEL );

  delay(1000);}

  
}

gamy_err InitESPNow( esp_now_send_cb_t OnDataSent, esp_now_recv_cb_t OnDataRecv )
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if( OnDataSent == NULL || OnDataRecv == NULL )
    return GAMY_INVALID_ARGUMENTS;

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return GAMY_FAIL;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer, since the master device will always broadcast data thus broadcast peer device is added here
  esp_now_add_peer(masterMacAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

static volatile uint8_t packet_sent = 0;
static volatile uint8_t packet_sent_flag = 0;

void SetPacketVars( uint8_t pack_sent, uint8_t pack_sent_flag ) {
  packet_sent = pack_sent;
  packet_sent_flag = pack_sent_flag;
}

gamy_err ESPNowSend( uint8_t * node, uint8_t * message, uint8_t len)
{
  packet_sent = packet_sent_flag = 0;
  esp_now_send( node, message, len );
  while( !packet_sent_flag ) delay(1);
  if( !packet_sent ) return GAMY_SUCCESS;
  else return GAMY_FAIL;
}

void RGBY( uint8_t R, uint8_t G, uint8_t B, uint8_t Y )
{
  if( R ) digitalWrite( RED_TEAM_GPIO, RELAY_ON_LEVEL );
  else digitalWrite( RED_TEAM_GPIO, RELAY_OFF_LEVEL );

  if( G ) digitalWrite( GREEN_TEAM_GPIO, RELAY_ON_LEVEL );
  else digitalWrite( GREEN_TEAM_GPIO, RELAY_OFF_LEVEL );

  if( B ) digitalWrite( BLUE_TEAM_GPIO, RELAY_ON_LEVEL );
  else digitalWrite( BLUE_TEAM_GPIO, RELAY_OFF_LEVEL );

  if( Y )digitalWrite( YELLOW_TEAM_GPIO, RELAY_ON_LEVEL );
  else digitalWrite( YELLOW_TEAM_GPIO, RELAY_OFF_LEVEL );

  digitalWrite( BUZZER_GPIO, LED_ON_LEVEL );
  delay( BUZZER_ON_PERIOD_MS );
  digitalWrite( BUZZER_GPIO, LED_OFF_LEVEL );
}

void RGBYReset()
{
  digitalWrite( RED_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( GREEN_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( BLUE_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( YELLOW_TEAM_GPIO, RELAY_OFF_LEVEL );
  digitalWrite( BUZZER_GPIO, LED_OFF_LEVEL );

  delay( RESET_PERIOD_MS );
}
