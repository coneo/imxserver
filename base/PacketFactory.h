#ifndef __PACKFACTORY_H__
#define __PACKFACTORY_H__

#include "Packet.h"


class PacketFactory
{
public:
	
	virtual ~PacketFactory() { }

	virtual Packet* 	CreatePacket() = 0;

	virtual PacketID_t	GetPacketID() const = 0;

	virtual UINT		GetPacketMaxSize() const = 0;

};

#endif
