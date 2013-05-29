
#ifndef __PACKETFACTORY_MANAGER_H__
#define __PACKETFACTORY_MANAGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#ifndef _IMX_CLIENT
	#define _IMX_CLIENT
#endif

#ifndef _IMX_LOGIN
	#define _IMX_LOGIN   /*_FOX_GATEWAY*/
#endif


class PacketFactoryManager
{
public:
	PacketFactoryManager( );

	~PacketFactoryManager( );

private:
	void		AddFactory( PacketFactory* pFactory );

public:
	bool		Init( );

	Packet*		CreatePacket( PacketID_t packetID);

	UINT		GetPacketMaxSize( PacketID_t packetID );

	void		RemovePacket( Packet* pPacket );

private:

	PacketFactory **		m_Factories;

	USHORT					m_Size;

public:
	UINT*			m_pPacketAllocCount;

};

extern PacketFactoryManager*	g_pPacketFactoryManager;

#endif
