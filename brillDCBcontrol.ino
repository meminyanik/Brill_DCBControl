/*
This program drives the Brill DCB for initial test

*/

//Global Include Files
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>
#include <WiFi.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Local Include Files
#include "Config.h"
#include "cMsgUdpRPhone.h"
#include "cMsgUdpTPhone.h"

WiFiUDP udp;
cMsgUdpRPhone MsgUdpRPhone;
cMsgUdpTPhone MsgUdpTPhone;

// the setup function runs once when you press reset or power the board
void setup() {

	//WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

	Serial.begin(SERIAL_COM_SPEED);
	Serial.println("");
	Serial.println("Serial communication is started.");

	delay(2000);
	connectToWiFi();

	//Processor 0 Tasks
	xTaskCreatePinnedToCore(task_serial, "task_serial", 1024, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(task_UDP, "task_UDP", 1600, NULL, 2, NULL, 0);
}

// the loop function runs over and over again until power down or reset
void loop() {
	vTaskSuspend(NULL);
}


void task_serial(void *parameter)
{
	
	while (true)
	{

		Serial.print("Healthy serial communication: ");
		Serial.println(MsgUdpRPhone.message.timeStamp);

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
	return;
}

void task_UDP(void * parameter)
{
	int udp_task_counter = 0;

	while (true)
	{
		preparePhoneUDPmessages();

		if (wifi_connected)
		{
			if (udp_connected)
			{
				if (udp.beginPacket(DEFAULT_PHONE_IP, UDP_PORT) != 1)
				{
					Serial.println("Could not begin UDP packet");
				}
				else
				{
					udp.write(MsgUdpRPhone.dataBytes, sizeof(MsgUdpRPhone.dataBytes));

					if (udp.endPacket() != 1)
					{
						Serial.println("UDP Packet could not send");
					}
					else
					{
						//Serial.println(millis());
					}
				}
			}
		}


		//UDP RX Operations
		if (wifi_connected && udp_connected && (udp_task_counter % 15 == 0))
		{

			if (udp.parsePacket() >= sizeof(MsgUdpTPhone.dataBytes))
			{
				udp.read(MsgUdpTPhone.dataBytes, sizeof(MsgUdpTPhone.dataBytes));
				MsgUdpTPhone.setPacket();

				updatePhoneUdpMsgVars();

			}

		}

		udp_task_counter++;

		if (udp_task_counter == 6000)
			udp_task_counter = 0;

		delay(10 / portTICK_PERIOD_MS);

	}
	vTaskDelete(NULL);
	return;
}


void connectToWiFi()
{

#ifdef BRILL_AP

	Serial.println();
	Serial.print("WiFi starting as access point:");
	Serial.println(BRILL_AP_NAME);
	WiFi.softAP(BRILL_AP_NAME, BRILL_AP_PASS);

	Serial.println("IP address: ");
	Serial.println(WiFi.softAPIP());


	wifi_connected = true;
	connectUdp();

#else
	// delete old config
	WiFi.disconnect(true);
	//register event handler
	WiFi.onEvent(WiFiEvent);

	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);

	WiFi.begin(WIFI_SSID, WIFI_PASS);
	unsigned long startTimeWifi = millis();
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		if (millis() - startTimeWifi > WIFI_CONNECTION_THRESHOLD)
		{
			Serial.println("Wifi connection timed out!");
			break;
		}
	}

	if (WiFi.status() == WL_CONNECTED)
	{

		wifi_connected = true;
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
		connectUdp();

	}
	else
	{
		wifi_connected = false;
		Serial.println("Wifi could not be connected!");
	}
#endif // BRILL_AP
}

void connectUdp()
{

#ifdef BRILL_AP
	if (udp.begin(WiFi.softAPIP(), UDP_PORT) == 1)
	{
		udp.clearWriteError();
		udp_connected = true;
		Serial.print("UDP Started on IP:");
		Serial.print(WiFi.softAPIP());
		Serial.print("@");
		Serial.println(UDP_PORT);
	}
	else
	{
		udp_connected = false;
		Serial.println("UDP could not be started!");
	}
#else
	if (udp.begin(WiFi.localIP(), UDP_PORT) == 1)
	{
		udp.clearWriteError();
		udp_connected = true;
		Serial.print("UDP Started on IP:");
		Serial.print(WiFi.localIP());
		Serial.print("@");
		Serial.println(UDP_PORT);
	}
	else
	{
		udp_connected = false;
		Serial.println("UDP could not be started!");
	}
#endif // BRILL_AP
}

void WiFiEvent(WiFiEvent_t event) {
	switch (event) {
	case SYSTEM_EVENT_STA_GOT_IP:
		//When connected set 
		Serial.print("Event Handler: WiFi connected! IP address: ");
		Serial.println(WiFi.localIP());
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		Serial.println("Event Handler: WiFi lost connection");
		//connectToWiFi();
		break;
	}
}

void preparePhoneUDPmessages()
{
	MsgUdpRPhone.message.timeStamp = millis();
	MsgUdpRPhone.message.phoneCommandAcknowledge = phoneCommandResponse;

	MsgUdpRPhone.getPacket();
}

void updatePhoneUdpMsgVars()
{
	phoneCommandResponse = MsgUdpTPhone.message.phoneCommand;
}