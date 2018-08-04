#include "cMsgUdpRPhone.h"
cMsgUdpRPhone::cMsgUdpRPhone()
{
	//If required message member initialization can be done here
}

//Class destructor
cMsgUdpRPhone::~cMsgUdpRPhone()
{
}

//This function should be used just before reading dataBytes of message.
//Data Array members starting from 0 to message struct size are set after the function call
void cMsgUdpRPhone::getPacket()
{
	//Sending Side
	memcpy(dataBytes, &message, sizeof(message));
}


//This function should be used just after a new dataPacket in order to set all the struct members
void cMsgUdpRPhone::setPacket()
{
	//Receiving Side
	memcpy(&message, dataBytes, sizeof(message));
}