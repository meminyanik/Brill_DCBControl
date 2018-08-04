/*
This program drives the Brill DCB for initial test

*/

#pragma once

#define		SERIAL_COM_SPEED				115200

//Wifi, UDP Definitions
//If it is desired to make the Brill as Access Point, uncomment the following line
#define		BRILL_AP

#define		BRILL_AP_NAME					"BrillDCB"
#define		BRILL_AP_PASS					"brilldcb"

// Phone IP Setting and possible SSIDs
//#define		DEFAULT_PHONE_IP		"255.255.255.255" // Boradcast for all networks
#define		DEFAULT_PHONE_IP		"192.168.4.255" // BRILL_AP Broadcast for internal network

#define		WIFI_SSID						"WIFI SSID"
#define		WIFI_PASS						"WIFIPassword"

#define		UDP_PORT						8080

#define		WIFI_CONNECTION_THRESHOLD	 10000

uint32_t udpLastMessageTime;

bool wifi_connected = false;
bool udp_connected = false;
int wifi_connection_attempt = 0;

bool phoneCommandResponse = false;