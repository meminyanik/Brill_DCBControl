#pragma once
#include <string.h>
using namespace std;

#pragma pack(push, 1)
struct structMsgUdpRPhone
{
	unsigned int timeStamp;

	unsigned int phoneCommandAcknowledge;

};
#pragma pack(pop)


class cMsgUdpRPhone
{
public:
	cMsgUdpRPhone();
	structMsgUdpRPhone message;
	unsigned char dataBytes[sizeof(message)];
	void getPacket();
	void setPacket();
	~cMsgUdpRPhone();
};
