#include "stdafx.h"
/**
 *\Version:0.0.1
 *\Date:2013-3-29
 *\Author: 
 */

#include "ConnectManager.h"
#include "Log.h"

ConnectManager * g_pConnectManager = NULL;

ConnectManager::ConnectManager( )
{
	if( !g_pGatewayPlayerManager )
	{
		g_pGatewayPlayerManager = new GatewayPlayerManager;
		//assert
	}
	g_pLog->log_debug("%u,%u,g_pGatewayPlayerManager Create ... OK",0,0);
	m_Active = false;	
}


ConnectManager::~ConnectManager( )
{
	SAFE_DELETE(g_pGatewayPlayerManager);
}

bool ConnectManager::Init( )
{
	bool ret = g_pGatewayPlayerManager->Init( );
	if( !ret )
	{
		g_pLog->log_debug("%u,%u,Init GatewayPlayerManager Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pGatewayPlayerManager Init ... OK",0,0);

	m_Active = true;

	return true;
}

void ConnectManager::run( )
{
	while(IsActive() )
	{
		bool ret = false;
		MySleep(100);

		ret = g_pGatewayPlayerManager->Select( );
		Assert(ret);
		
		ret = g_pGatewayPlayerManager->ProcessExceptions( );
		Assert(ret);

		ret = g_pGatewayPlayerManager->ProcessInputs( );
		Assert(ret);

		ret = g_pGatewayPlayerManager->ProcessOutputs( );
		Assert(ret);

		ret = g_pGatewayPlayerManager->ProcessCommands( );
		Assert(ret);

		ret = g_pGatewayPlayerManager->HeartBeat( );
		Assert(ret);
	};
	
	Quit( );
}

void ConnectManager::Quit( )
{
	g_pGatewayPlayerManager->RemoveAllPlayers( );
	std::cout<<"##ConnectManager::quit"<<std::endl;	
}
