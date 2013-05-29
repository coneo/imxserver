/**
 *Version:0.0.1
 *Date:2013-3-23
 *Author: 
 */

#include "PlayerPool.h"
#include "ConnectManager.h"
#include "GatewayServer.h"
#include "PacketFactoryManager.h"
#include "ServerThreadManager.h"
#include "ServerManager.h"
#include "Log.h"

GatewayServer::GatewayServer( )
	:m_bExited(false)
{
	
}

GatewayServer::~GatewayServer( )
{

}

bool GatewayServer::Init( )
{
	bool 	bRet = false;

	bRet = NewGatewayServer( );
	if(!bRet)
	{
		g_pLog->log_debug("%u,%u,NewGatewayServer Failed",0,0);
		return false;
	}
	bRet = InitGatewayServer( );
	if(!bRet)
	{
		g_pLog->log_debug("%u,%u,InitGatewayServer Failed",0,0);
		return false;
	}
	m_bExited = false;
	return true;
}

bool GatewayServer::Loop( )
{	
	bool bRet = false;

	g_pLog->log_debug("%u,%u,g_pConnectManager->start ... OK",0,0);
	g_pConnectManager->start( );

	g_pLog->log_debug("%u,%u,g_pServerThreadManager->start ... OK",0,0);
	bRet = g_pServerThreadManager->Start( );	
	Assert(bRet);

	//守护线程	
	while(true)
	{
		MySleep(100);
	}

	return bRet;
}

bool GatewayServer::Exit( )
{
	SAFE_DELETE(g_pPlayerPool);
	g_pLog->log_debug("%u,%u,Exit g_pPlayerPool...OK",0,0);

	SAFE_DELETE(g_pPacketFactoryManager);
	g_pLog->log_debug("%u,%u,Exit g_pPacketFactoryManager...OK",0,0);

	SAFE_DELETE(g_pConnectManager);
	g_pLog->log_debug("%u,%u,Exit g_pConnectManager...OK",0,0);
	
	return true;
}

void GatewayServer::Stop( )
{	
	if(g_pConnectManager)
	{
		g_pConnectManager->stop( );
	}
}

bool GatewayServer::NewGatewayServer( )
{
	g_pPlayerPool	= new PlayerPool ;
	if(!g_pPlayerPool)
	{
		g_pLog->log_error("%u,%u,g_pPlayerPool Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pPlayerPool Create ... OK",0,0);

	g_pPacketFactoryManager = new PacketFactoryManager;
	if(!g_pPacketFactoryManager)
	{
		g_pLog->log_error("%u,%u,g_pPacketFactoryManager Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pPacketFactoryManager Create ... OK",0,0);

	g_pConnectManager = new ConnectManager;	
	if(!g_pConnectManager) 
	{
		g_pLog->log_error("%u,%u,g_pConnectManager Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pConnectManager Create ... OK",0,0);

	g_pServerThreadManager = new ServerThreadManager;
	if(!g_pServerThreadManager)
	{
		g_pLog->log_error("%u,%u,g_pServerThreadManager Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pServerThreadManager Create ... OK",0,0);

	g_pServerManager = new ServerManager;
	if(!g_pServerManager)
	{
		g_pLog->log_error("%u,%u,g_pServerManager Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pServerManager Create ... OK",0,0);
	
	return true;
}

bool GatewayServer::InitGatewayServer( )
{
	bool bRet	= false;

	bRet = g_pPlayerPool->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pPlayerPool->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pPlayerPool->Init ... OK",0,0);

	bRet = g_pPacketFactoryManager->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pPacketFactoryManager->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pPacketFactoryManager->Init ... OK",0,0);
		
	bRet = g_pConnectManager->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pConnectManager->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pConnectManager->Init ... OK",0,0);

	bRet = g_pServerThreadManager->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pServerThreadManager->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pServerThreadManager->Init ... OK",0,0);

	bRet = g_pServerManager->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pServerManager->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pServerManager->Init ... OK",0,0);
	g_pLog->log_error("%u,%u,error test.....",0,0);

	return true;
}
