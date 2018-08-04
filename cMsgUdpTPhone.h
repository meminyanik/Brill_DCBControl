#pragma once
#include <string.h>
using namespace std;

#pragma pack(push, 1)
struct structMsgUdpTPhone
{
	unsigned char phoneCommand;
};
#pragma pack(pop)


class cMsgUdpTPhone
{
public:
	cMsgUdpTPhone();
	structMsgUdpTPhone message;
	unsigned char dataBytes[sizeof(message)];
	void getPacket();
	void setPacket();
	~cMsgUdpTPhone();
};