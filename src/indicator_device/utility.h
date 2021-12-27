/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : utility.h
 * @brief : This file contains declarations for utility functions and classes.
*/
#ifndef UTILITY_H
#define UTILITY_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "indicator_device.h"

typedef enum gamy_err{ GAMY_FAIL = 1, GAMY_SUCCESS = 0, GAMY_INVALID_ARGUMENTS = 2 }gamy_err;

typedef enum gamy_team{ RED_TEAM = 1, BLUE_TEAM = 2, GREEN_TEAM = 3, YELLOW_TEAM = 4, NO_TEAM = 5, ALL_TEAM = 6, INVALID_TEAM = 0 }gamy_team;

typedef enum gamy_button_device_state{ GAMY_DISCONNECTED = 0, GAMY_CONNECTED = 1 }gamy_button_device_states;

typedef struct gamy_message{
  gamy_team team;
}gamy_message;

void enqueue( gamy_team );

gamy_team dequeue();

void empty();

void InitGeneral();

gamy_err InitESPNow( esp_now_send_cb_t, esp_now_recv_cb_t );

void SetPacketVars( uint8_t, uint8_t );

gamy_err ESPNowSend( uint8_t *, uint8_t *, uint8_t);

void RGBY( uint8_t , uint8_t , uint8_t , uint8_t );

void RGBYReset();

#endif
