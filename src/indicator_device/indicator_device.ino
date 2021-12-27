/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : indicator_device.cpp
 * @brief : This file contains source code for indicator device for quiz show system.
*/
#include "utility.h"

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static unsigned long current_time, last_time_stamp;
static gamy_team packet;

static gamy_button_device_state STATE;

static uint8_t masterMacAddress[] = MASTER_MAC_ADDRESS;

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Callback when data is sent */
void OnDataSent(uint8_t *, uint8_t );

/* Callback when data is received */
void OnDataRecv(uint8_t *, uint8_t *, uint8_t );

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Function for disconnected state operations */
void disconnected_state();

/* Function for connected state operations */
void connected_state();

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




void setup() {
  
  /* General initializations */
  InitGeneral();

  /* Uncomment the following for debugging purpose */
  Serial.println("General Initialized");

  /* ESP-NOW Related initializations */
  if ( InitESPNow( OnDataSent, OnDataRecv ) != GAMY_SUCCESS )
  {
    Serial.println("Failed to initialize ESP NOW");
    delay(1000);
    ESP.restart();
  }

  /* Initializing time stamp variables */
  current_time = last_time_stamp = millis();

  /* Initializing State variable */
  STATE = GAMY_DISCONNECTED;

  /* Uncomment the following for debugging purpose */
  Serial.println("Initialized Device");
}

void loop() {

   /* Get the current time in ms */
   current_time = millis();
   
   /* Get packet from the queue */
   packet = dequeue();

   switch( STATE )
   {
      case GAMY_DISCONNECTED :
       disconnected_state();
      break;

      case GAMY_CONNECTED :
        connected_state();
      break;
   }
   
}

/* Callback when data is sent */
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  SetPacketVars( sendStatus, 1 );
}

static gamy_message recvMsg;
/* Callback when data is received */
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  
  /* Uncomment the following 2 lines for debugging purposes */  
  //Serial.print("Bytes received: ");
  //Serial.println(len);

  for( uint8_t i=0; i<6; i++)
  {
    if( mac[i] != masterMacAddress[i] )
    {
      len = 0;        
    }
  }
  
  if( !len ) return;

  //Serial.println("Packet from master");

  /* If it's a packet from master device then enqueue M */
  memcpy( (uint8_t *)&recvMsg, incomingData, len );
  enqueue( recvMsg.team );

  //Serial.print( "Team : " );
  //Serial.println( recvMsg.team );
}

/* Function for disconnected state operations */
void disconnected_state()
{  
   if( packet != INVALID_TEAM )
   {
    STATE = GAMY_CONNECTED;
    last_time_stamp = current_time;
    digitalWrite( LED_GPIO, LED_ON_LEVEL );

    Serial.println( "Connected !" );
   }
   else if( current_time - last_time_stamp >= DISCONNECTED_TIMEOUT_MS )
   {
    Serial.println( "Disconnected timer expired, rebooting !" );
    delay(5000);
    ESP.restart();
   }
}

/* Function for connected state operations */
void connected_state()
{
  if( packet != INVALID_TEAM )
  {
    switch( packet )
    {
      case RED_TEAM:
        Serial.println( "Red Team" );
        RGBY( 1, 0, 0, 0 );
        empty();
      break;

      case BLUE_TEAM:
        Serial.println( "Blue Team" );
        RGBY( 0, 0, 1, 0 );
        empty();
      break;

      case GREEN_TEAM:
        Serial.println( "Green Team" );
        RGBY( 0, 1, 0, 0 );
        empty();
      break;

      case YELLOW_TEAM:
        Serial.println( "Yellow Team" );
        RGBY( 0, 0, 0, 1 );
        empty();
      break;

      case NO_TEAM:
        Serial.println( "Reset" );
        RGBYReset();
        empty();
      break;

      case ALL_TEAM:
        Serial.println( "Beacon!" );
      break;
    }
    
    last_time_stamp = current_time;
  }
  else if( current_time - last_time_stamp >= PACKET_WAIT_MS )
  {
    STATE = GAMY_DISCONNECTED;
    last_time_stamp = current_time;
    digitalWrite( LED_GPIO, LED_OFF_LEVEL );

    Serial.println( "Disconnected !");
  }
}
