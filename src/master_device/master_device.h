/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : master_device.h
 * @brief : This file contains configuration parameters for master device (used by quiz master) for quiz show system.
*/
#ifndef MASTER_DEVICE_H
#define MASTER_DEVICE_H

/*
 * CHANGE THIS !!
 * Following are the 802.11 MAC Addresses for the four button devices.
*/
#define RED_MAC_ADDRESS { 0xBC, 0xDD, 0xC2, 0x30, 0xB0, 0x52 } 
#define BLUE_MAC_ADDRESS { 0xDE, 0xFF, 0x12, 0x34, 0x56, 0x78 }
#define GREEN_MAC_ADDRESS { 0x9A, 0xBC, 0xDE, 0xFF, 0x12, 0x34 }
#define YELLOW_MAC_ADDRESS { 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF }

/*
 * This is the time period for beacon transmissions (in ms.)
*/
#define BEACON_MS 1000

/*
 * This is the number of times the beacon is transmitted at every time period. 
*/
#define MAX_BROADCASTS 5

/*
 * This is the interval between two consecutive beacon broadcasts.
*/
#define BROADCAST_INTERVAL_MS 50

/*
 * This is the period for which indication stays on (in ms.)
*/
#define INDICATION_DELAY_MS 5000

/*
 * Following are the GPIOs.
*/
#define BUTTON_GPIO 0
#define LED_GPIO 2

#define LED_ON_LEVEL HIGH
#define LED_OFF_LEVEL LOW

/*
 * The Serial port baud rate in bps.
*/
#define SERIAL_BAUDRATE 115200

/*
 * This is the button debounce period.
 * DON'T PLAY AROUND THIS WITH THIS SETTING !
*/
#define DEBOUNCE_MS 80

/*
 * This is the buffer length.
*/
#define BUFF_LENGTH 50

#endif
