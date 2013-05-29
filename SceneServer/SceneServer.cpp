/**
 *Version 0.0.1
 *Date 2013-4-15
 *Author
 */

#include "PlayerPool.h"
#include "OnlineUser.h"
#include "SceneServer.h"
#include "ConnectManager.h"
#include "PacketFactoryManager.h"
#include "Log.h"

SceneServer::SceneServer( )
	:m_bExited(false)
{
		
}

SceneServer::~SceneServer( )
{

}

bool SceneServer::Init( )
{
	g_pLog->log_debug("user:%u-id:%u-SceneServer Init",0,0);

	bool bRet = false;

	bRet = NewSceneServer( );
	if(!bRet)
	{
		Assert(false);
		return false;
	}
	bRet = InitSceneServer( );
	if(!bRet)
	{
		Assert(false);
		return false;
	}
	m_bExited = false;

	return true;
}

bool SceneServer::Loop( )
{
	g_pConnectManager->start( );

	//守护线程
	while(true)
	{
		MySleep(100);
	}

	return true;
}

bool SceneServer::Exit( )
{
	SAFE_DELETE(g_pPlayerPool);
	g_pLog->log_debug("%u,%u,释放g_pPlayerPool",0,0);

	SAFE_DELETE(g_pOnlineUser);
	g_pLog->log_debug("%u,%u,释放g_pOnlineUser",0,0);

	SAFE_DELETE(g_pPacketFactoryManager);
	g_pLog->log_debug("%u,%u,Exit g_pPacketFactoryManager",0,0);

	SAFE_DELETE(g_pConnectManager);
	g_pLog->log_debug("%u,%u,释放g_pConnectManager",0,0);

	return true;
}

void SceneServer::Stop( )
{
	if(g_pConnectManager)
	{
		g_pConnectManager->stop( );
	}
}

bool SceneServer::NewSceneServer( )
{
	g_pPlayerPool = new PlayerPool;
	Assert(g_pPlayerPool);
	if(!g_pPlayerPool)
	{
		g_pLog->log_error("%u,%u,PlayerPool 创建失败",0 ,0);
		return false;
	}

	g_pOnlineUser = new OnlineUser;
	Assert(g_pOnlineUser);
	if(!g_pOnlineUser)
	{
		g_pLog->log_error("%u,%u,OnlineUser 创建失败",0 ,0);
		return false;
	}

	g_pPacketFactoryManager = new PacketFactoryManager;
	if(!g_pPacketFactoryManager)
	{
		g_pLog->log_error("%u,%u,g_pPacketFactoryManager Create Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pPacketFactoryManager Create ... OK",0,0);

	g_pConnectManager = new ConnectManager;
	Assert(g_pConnectManager);
	if(!g_pConnectManager)
	{
		g_pLog->log_error("%u,%u,ConnectManager 创建失败",0 ,0);
		return false;
	}

	return true;
}

bool SceneServer::InitSceneServer( )
{
	bool bRet = false;

	bRet = g_pPlayerPool->Init( );
	Assert(bRet);

	bRet = g_pOnlineUser->Init( );
	Assert(bRet);

	bRet = g_pPacketFactoryManager->Init( );
	Assert(bRet);

	bRet = g_pConnectManager->Init( );
	Assert(bRet);

	return true;
}
