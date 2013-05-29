#include "stdafx.h"

/**
 *version 0.0.1
 *date 2013-4-17
 *author
 */

#include "ConnectManager.h"
#include "Log.h"

ConnectManager * g_pConnectManager = NULL;

ConnectManager::ConnectManager( )
{
	if(!g_pScenePlayerManager)
	{
		g_pScenePlayerManager = new ScenePlayerManager;
		Assert(g_pScenePlayerManager);
	}
	m_Active = false;
}


ConnectManager::~ConnectManager( )
{
	SAFE_DELETE(g_pScenePlayerManager);	
}

bool ConnectManager::Init( )
{
	bool ret = g_pScenePlayerManager->Init( );
	if(!ret)
	{
		g_pLog->log_error("user:%u,id:%u,ConnectManager Init 失败!",0,0);
		return false;
	}

	m_Active = true;

	return true;
}

void ConnectManager::run( )
{
	while(IsActive())
	{
		bool ret = false;

		MySleep(100);

		ret = g_pScenePlayerManager->Select( );
		Assert(ret);

		ret = g_pScenePlayerManager->ProcessExceptions( );
		Assert(ret);

		ret = g_pScenePlayerManager->ProcessInputs( );
		Assert(ret);

		ret = g_pScenePlayerManager->ProcessOutputs( );
		Assert(ret);

		ret = g_pScenePlayerManager->ProcessCommands( );
		Assert(ret);

		ret = g_pScenePlayerManager->HeartBeat( );
		Assert(ret);
	};

	Quit( );
}

void ConnectManager::Quit( )
{
	g_pScenePlayerManager->RemoveAllPlayers( );
	g_pLog->log_debug("user:%u,id:%u,退出ConnectManager",0,0);
}
