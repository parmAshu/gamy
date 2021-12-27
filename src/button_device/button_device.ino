/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : button_device.cpp
 * @brief : This file contains source code for button device for quiz show system.
*/
#include "utility.h"

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static unsigned long current_time, last_time_stamp;
static char packet;

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

/**/
void PressPacket();

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

/* Callback when data is received */
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  
  /* Uncomment the following 2 lines for debugging purposes */  
  // Serial.print("Bytes received: ");
  // Serial.println(len);

  for( uint8_t i=0; i<6; i++)
  {
    if( mac[i] != masterMacAddress[i] )
    {
      len = 0;        
    }
  }
  
  if( !len ) return;

  /* If it's a packet from master device then enqueue M */
  enqueue( 'M' );
}

/**/
void PressPacket()
{
  gamy_message msg;
  msg.team = DEVICE_TEAM;

  /* Uncomment the following line for debugging purpose */
  // Serial.println("Pressed, sending packets...");

  uint8_t i=1;
  for( ; i<=MAX_RETRIES; i++)
  {
    if( ESPNowSend( masterMacAddress, (uint8_t *)&msg, sizeof(msg) ) == GAMY_SUCCESS ) break;
    if( i == MAX_RETRIES ) i = 0;
    delay( RESEND_MS );
  }

  /* Uncomment the following line for debugging purpose */
  // Serial.println("Waiting for release...");

  uint32_t indication_counter = 0;
  while( !digitalRead( BUTTON_GPIO ) )
  {
    delay(1);
    indication_counter++;
  }

  /* Uncomment the following line for debugging purpose */
  // Serial.println("Released!");
      
  if( !i )
  {
    // Serial.println("Failed to send - Disconnected ");
    STATE = GAMY_DISCONNECTED;
  }

  if( indication_counter >= INDICATION_MS )
  {
    if( STATE == GAMY_CONNECTED )
    {
      LED( 10, 200 );
    }
    else
    {
      LED( 5, 250 );
    }
  }
  
  last_time_stamp = current_time;
}

/* Function for disconnected state operations */
void disconnected_state()
{
   if( !digitalRead(BUTTON_GPIO) )
   {
     delay( DEBOUNCE_MS );
     if( !digitalRead( BUTTON_GPIO ) )
     {
      /* Indicate disconnected state */
      LED( 5, 250 ); 
     }
   }
   
   if( packet == 'M' )
   {
    STATE = GAMY_CONNECTED;
    last_time_stamp = current_time;
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
  if( !digitalRead( BUTTON_GPIO ) )
  {
    delay( DEBOUNCE_MS );
    if( !digitalRead( BUTTON_GPIO ) )
    {
      PressPacket();
    }
  }
        
  if( packet == 'M' )
  {
    last_time_stamp = current_time;
  }
  else if( current_time - last_time_stamp >= PACKET_WAIT_MS )
  {
    STATE = GAMY_DISCONNECTED;
    last_time_stamp = current_time;
  }
}
