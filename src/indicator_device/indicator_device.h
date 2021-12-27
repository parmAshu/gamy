/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : indicator_device.h
 * @brief : This file contains configuration parameters for indicator device for quiz show system.
*/
#ifndef INDICATOR_DEVICE_H
#define INDICATOR_DEVICE_H

/*
 * CHANGE THIS FOR EVERY SYSTEM !!
 * The 802.11 MAC Address of the master device.
*/
#define MASTER_MAC_ADDRESS { 0x84, 0xF3, 0xEB, 0x31, 0xEC, 0x8B }

/*
 *  The time period for which device will stay in disconnected state (in ms).
 *  After this time period the device will reboot.
*/
#define DISCONNECTED_TIMEOUT_MS 60000

/*
 * The time period for which device will stay in connected state (in ms) without recieving a packet from master device.
 * After this time period the device will go to disconnected state.
*/
#define PACKET_WAIT_MS 10000

/*
 * The minimum amount of time for which the indication will stay on.
*/
#define INDICATION_MS 8000

/*
 * The time period for which buzzer will stay on.
*/
#define BUZZER_ON_PERIOD_MS 5000

/*
 * The time period taken for reset operation
*/
#define RESET_PERIOD_MS 2000

/*
 * Various GPIOs on the indicator device.
*/
#define LED_GPIO 2
#define BUZZER_GPIO 4
#define RED_TEAM_GPIO 5 
#define GREEN_TEAM_GPIO 12
#define BLUE_TEAM_GPIO 13
#define YELLOW_TEAM_GPIO 14

#define LED_ON_LEVEL HIGH
#define LED_OFF_LEVEL LOW

#define RELAY_ON_LEVEL LOW
#define RELAY_OFF_LEVEL HIGH

/*
 * Serial port baud rate in bps.
*/
#define SERIAL_BAUDRATE 115200

/*
 * Buffer length.
*/
#define BUFF_LENGTH 50

/*  Button Debounce time in ms.
 *  TECHNICAL SETTING, DO NOT PLAY AROUND WITH IT!
*/
#define DEBOUNCE_MS 80

#endif
