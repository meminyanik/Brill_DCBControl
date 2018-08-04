#include "cMsgUdpTPhone.h"
cMsgUdpTPhone::cMsgUdpTPhone()
{
	//If required message member initialization can be done here
}

//Class destructor
cMsgUdpTPhone::~cMsgUdpTPhone()
{
}

//This function should be used just before reading dataBytes of message.
//Data Array members starting from 0 to message struct size are set after the function call
void cMsgUdpTPhone::getPacket()
{
	//Sending Side
	memcpy(dataBytes, &message, sizeof(message));
}


//This function should be used just after a new dataPacket in order to set all the struct members
void cMsgUdpTPhone::setPacket()
{
	//Receiving Side
	memcpy(&message, dataBytes, sizeof(message));
}