/*
 * @author : Ashutosh Singh Parmar
 * @date : 17-12-2021
 * @file : button_device.h
 * @brief : This file contains configuration parameters for button device for quiz show system.
*/
#ifndef BUTTON_DEVICE_H
#define BUTTON_DEVICE_H

/*
 * CHANGE THIS !!
 * This macro defines the team which this device will represent.
*/
#define DEVICE_TEAM YELLOW_TEAM

/*
 * CHANGE THIS !!
 * This is the 802.11 MAC Address of the master device.
*/
#define MASTER_MAC_ADDRESS { 0x84, 0xF3, 0xEB, 0x31, 0xEC, 0x8B }

/*
 * This is the period for which the device can stay disconnected without rebooting.
*/
#define DISCONNECTED_TIMEOUT_MS 60000

#define PACKET_WAIT_MS 8000

/*
 * This is the maximum number of times this device will try to send the team to master device.
 * If there is no ACK from the master after these many tries then, device will go to disconnected state.
*/
#define MAX_RETRIES 5

/*
 * Interval between two consecutive transmissions (in ms).
*/
#define RESEND_MS 10

/*
 * This is the minimum time period (in ms) for which indication stays on.
*/
#define INDICATION_MS 8000

/*
 * These are the GPIO numbers.
*/
#define BUTTON_GPIO 0
#define LED_GPIO 2

#define LED_ON_LEVEL HIGH
#define LED_OFF_LEVEL LOW

/*
 * This is the Serial port baud rate in bps.
*/
#define SERIAL_BAUDRATE 115200

/*
 * This is the debounce time for the button in the device (in ms).
 * DON'T PLAY AROUND WITH THIS SETTING !!
*/
#define DEBOUNCE_MS 80

/*
 * Buffer Length.
*/
#define BUFF_LENGTH 50

#endif
