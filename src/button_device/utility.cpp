/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : utility.cpp
 * @brief : This file contains some utility functions and classes.
*/
#include "utility.h"

static uint8_t masterMacAddress[] = MASTER_MAC_ADDRESS;

static char buff[BUFF_LENGTH];
static uint8_t filled;
static uint8_t inPos=0;
static uint8_t outPos=0;

void enqueue( char team )
{
  if( filled < BUFF_LENGTH )
  {
    buff[inPos++] = team;
    inPos = ( inPos == BUFF_LENGTH ) ? 0 : inPos;

    ATOMIC() {
      filled++;
    }
  }
}

char dequeue()
{
  char team = 'E';
  if( filled )
  {
    team = buff[outPos++];
    outPos = ( outPos == BUFF_LENGTH ) ? 0 : outPos;

    ATOMIC() {
      filled--;
    }
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

  pinMode( BUTTON_GPIO, INPUT_PULLUP );

  pinMode( LED_GPIO, OUTPUT );

  digitalWrite( LED_GPIO, LED_OFF_LEVEL );
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

void LED( uint8_t count, uint16_t dly )
{
  for(; count > 0; count--)
  {
    digitalWrite( LED_GPIO, LED_ON_LEVEL );
    delay(dly);
    digitalWrite( LED_GPIO, LED_OFF_LEVEL );
    delay(dly);
  }
}
