#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "Log.h"

//CL && LC
#if defined (_IMX_CLIENT) && defined (_IMX_LOGIN)
//CL
#include "CLAskLogin.h"
#include "imxGSConnect.h"
#include "imxCGPosition.h"
#include "imxGCClientChangeLocation.h"
#endif


using namespace Packets;

PacketFactoryManager*	g_pPacketFactoryManager = NULL;

PacketFactoryManager::PacketFactoryManager( )
{
	m_Factories = NULL;
	m_Size = PACKET_MAX ;

	Assert (m_Size > 0);
	
	m_Factories = new PacketFactory*[m_Size];
	if(!m_Factories)
		std::cout<<"new m_Factories failed\n";

	m_pPacketAllocCount = new UINT[m_Size];
	//assert
	
	for(INT i=0; i<m_Size; i++)
	{
		m_Factories[i] = NULL;
		m_pPacketAllocCount[i] = 0;
	}

}

PacketFactoryManager::~PacketFactoryManager( )
{
	//assert (m_Factories != NULL);
	//
	for(INT i=0; i<m_Size; i++)
	{
		SAFE_DELETE(m_Factories[i]);
	}

	SAFE_DELETE_ARRAY(m_Factories);
	SAFE_DELETE_ARRAY(m_pPacketAllocCount);
}

bool PacketFactoryManager::Init( )
{
//CL && LC
#if defined (_IMX_CLIENT) && defined (_IMX_LOGIN)

	AddFactory( new CLAskLoginFactory() );
	AddFactory( new imxGSConnectFactory() );
	AddFactory( new imxCGPosition_exFactory() );
	AddFactory( new imxCGPositionFactory() );
	AddFactory( new imxGCClientChangeLocationFactory() );

#endif

	return true;
}

void PacketFactoryManager::AddFactory( PacketFactory* pFactory )
{
	if(m_Factories[pFactory->GetPacketID()] != NULL )
	{
		return;
	}

	m_Factories[pFactory->GetPacketID()] = pFactory ;
}

Packet* PacketFactoryManager::CreatePacket( PacketID_t packetID )
{
	std::cout<<"CreatePacket start...packetID:"<<packetID<<std::endl;
	if(packetID >= m_Size || m_Factories[packetID]==NULL)
	{
		Assert(false);
		std::cout<<"CreatePacket failed\n";
		return NULL;
	}

	Packet* pPacket = NULL;

	pPacket = m_Factories[packetID]->CreatePacket( );
	if(!pPacket)
	{
		std::cout<<"CreatePacket NULL"<<std::endl;
		return NULL;

	}

	m_pPacketAllocCount[packetID]++;

	g_pLog->log_debug("CreatePacketSuccess packID:%u",packetID);
	
	return pPacket;
}

UINT PacketFactoryManager::GetPacketMaxSize(PacketID_t packetID)
{
	if(packetID>=m_Size || m_Factories[packetID]==NULL)
	{
		std::cout<<"packetID超过最大值或者消息未注册\n";

		return 0;
	}

	UINT iRet = m_Factories[packetID]->GetPacketMaxSize( );

	return iRet;
}

void PacketFactoryManager::RemovePacket(Packet* pPacket)
{
	if(pPacket==NULL)
	{
		//assert
		return ;
	}
	
	PacketID_t packetID = pPacket->GetPacketID( );
	if(packetID >= m_Size)
	{
		//assert
		return;
	}

	SAFE_DELETE(pPacket);
	m_pPacketAllocCount[packetID]--;

	return;
}
