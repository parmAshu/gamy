/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : master_device.cpp
 * @brief : This file contains source code for master device (used by quiz master) for quiz show system.
*/

#include "utility.h"

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Variable to store current state */
static gamy_master_device_state STATE;

/* Time-stamp variables */
static unsigned long last_time_stamp;
static unsigned long current_time;
static unsigned long delay_start_time;

/* Variable to store the team name read from queue */
static gamy_team _team;

static gamy_message broadcastMsg;
static uint8_t broadcastMsgLen;

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Callback when data is sent */
void OnDataSent(uint8_t *, uint8_t );

/* Callback when data is received */
void OnDataRecv(uint8_t *, uint8_t *, uint8_t );

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Function to broadcast heartbeat beacon */
void Beacon();

/* Function to wait for button press */
void WaitForPress();

/* Function to broadcast teams for the indication device */
void BroadcastTeam( gamy_team );

/* Function to broadcast reset message for the indication device */
void BroadcastReset();

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Function for normal state operations */
void normal_state();

/* Function for delay state operations */
void delay_state();

/* Function for wait state operations */
void wait_state();

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




void setup() {

  /* General initializations */
  InitGeneral();

  /* Uncomment the following for debugging purpose */
  Serial.println("General Initialized");

  /* ESP-NOW Related initializations */
  if ( InitESPNow( OnDataSent, OnDataRecv ) != GAMY_SUCCESS )
  {
    /* If initialization fails, then reboot */
    Serial.println("Failed to initialize ESP NOW");
    delay(1000);
    ESP.restart();
  }

  /* Initializing time stamp variables */
  current_time = last_time_stamp = millis();
  
  /* Initializing state variable */
  STATE = GAMY_NORMAL;

  broadcastMsg.team = ALL_TEAM;
  broadcastMsgLen = sizeof(broadcastMsg);

  /* Uncomment the following for debugging purpose */
  Serial.println("Initialized Device");
}

void loop() 
{
  /* Get the current time in ms */
  current_time = millis();

  /* Get the team name from the queue */
  _team = dequeue();
  
  switch( STATE )
  {
    case GAMY_NORMAL:
      normal_state();
    break;

    case GAMY_DELAY:
      delay_state();
    break;

    case GAMY_WAIT:
      wait_state();
    break;
  }
}

/* Callback when data is sent */
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  SetPacketVars( sendStatus, 1 );
}

/* Callback when data is received */
static gamy_message ESP_NOW_MSG;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) 
{
  /* Uncomment the following for debugging purposes */
  // Serial.print("Bytes received: ");
  // Serial.println(len);

  if( !len ) return;

  memcpy( &ESP_NOW_MSG, incomingData, len );

  switch( ESP_NOW_MSG.team )
  {
    case RED_TEAM :
    //Serial.println("Red Team");
    enqueue( RED_TEAM );
    break;

    case BLUE_TEAM :
    //Serial.println("Blue Team");
    enqueue( BLUE_TEAM );
    break;

    case GREEN_TEAM :
    //Serial.println("Green Team");
    enqueue( GREEN_TEAM );
    break;

    case YELLOW_TEAM :
    //Serial.println("Yellow Team");
    enqueue( YELLOW_TEAM );
    break;
  }
}

/* Function to broadcast heartbeat beacon */
void Beacon()
{
  // If the timer is expired then,
  if( current_time - last_time_stamp >= BEACON_MS )
  {
    /* Uncomment the following for debugging purposes */
    // Serial.println("broadcast message");

    /* Broadcast beacon */
    ESPNowBroadcast( (uint8_t *)&broadcastMsg, broadcastMsgLen );

    /* Reset the timer*/
    last_time_stamp = current_time;
  }
}

/* Function to wait for button press */
void WaitForPress()
{
  if( !digitalRead( BUTTON_GPIO ) )
  {
    delay( DEBOUNCE_MS );
    if( !digitalRead( BUTTON_GPIO ) )
    { 
      //Serial.println("Pressed, waiting for release");

      /* Wait for button release */
      while( 1 )
      {
        if( digitalRead( BUTTON_GPIO ) )
        {
          delay( DEBOUNCE_MS );
          if( digitalRead( BUTTON_GPIO ) )
          {
            //Serial.println("Released");
            break;
          }
        }
        delay(10);
      }

      BroadcastReset();
      STATE = GAMY_NORMAL;
      empty();
      digitalWrite( LED_GPIO, LED_OFF_LEVEL );
    }
  }
  
}

/* Function to broadcast team */
void BroadcastTeam( gamy_team tm )
{
  /* Variable for broadcasting team messages */
  gamy_message msg;
  
  /* Broadcast the team for indication device */
  msg.team = tm;
  
  for( uint8_t i=1; i<=MAX_BROADCASTS; i++)
  {
     ESPNowBroadcast( (uint8_t *)&msg, sizeof(msg) );
     delay(BROADCAST_INTERVAL_MS);
  }
}

/* Function to broadcast reset message for indication device */
void BroadcastReset()
{
  gamy_message msg;
  msg.team = NO_TEAM;
  
  for( uint8_t i=1; i<=MAX_BROADCASTS; i++)
  {
     ESPNowBroadcast( (uint8_t *)&msg, sizeof(msg) );
     delay(BROADCAST_INTERVAL_MS);
  }
}

/* Function for normal state functions */
void normal_state()
{
  /* Check if there is a packet from any button device */
  if( _team != INVALID_TEAM )
  {
    /* Uncomment the following 2 lines for debugging purpose */
    //Serial.print("Team : ");
    //Serial.println(_team);
    
    BroadcastTeam( _team );

    /* Change the state to wait state */
    STATE = GAMY_DELAY;
    delay_start_time = current_time;
    digitalWrite( LED_GPIO, LED_ON_LEVEL );
    
   }
   /* Broadcast heartbeat beacon */
   Beacon();
}

/* Function for delay state functions */
void delay_state()
{
  if( current_time - delay_start_time >= INDICATION_DELAY_MS )
  {
    STATE = GAMY_WAIT;
  }
  /* Broadcast heartbeat beacon */
  Beacon();
}

/* Function for wait state functions */
void wait_state()
{
  WaitForPress();
  Beacon();
}
