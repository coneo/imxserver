#include "stdafx.h"

#include "LoginServer.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "Log.h"

LoginServer 		g_LoginServer;

LoginServer::LoginServer( )
{

}

LoginServer::~LoginServer( )
{
	
}

bool LoginServer::Init( )
{
	bool bRet = false;

	bRet = NewStaticManager( );
	Assert(bRet);

	bRet = InitStaticManager( );
	Assert(bRet);

	return true;
}

bool LoginServer::Loop( )
{
	g_pLog->log_debug("%u,%u,Loop....",0,0);	
	g_pServerManager->Loop( );

	return true;
}

bool LoginServer::Exit( )
{
	bool bRet = false;

	bRet = DelStaticManager( );
	Assert(bRet);

	return true;
}

bool LoginServer::NewStaticManager( )
{
	g_pServerManager = new ServerManager;
	if(!g_pServerManager)
	{
		g_pLog->log_error("%u,%u,New ServerManager ... failed",0,0);
	}
	g_pLog->log_debug("%u,%u,New ServerManager ... OK",0,0);



	g_pPlayerPool = new PlayerPool;
	if(!g_pPlayerPool)
	{
		g_pLog->log_error("%u,%u,New PlayerPool ... failed",0,0);
	}
	g_pLog->log_debug("%u,%u,New PlayerPool ... OK",0,0);



	g_pPacketFactoryManager = new PacketFactoryManager;
	if(!g_pPacketFactoryManager)
	{
		g_pLog->log_error("%u,%u,New PacketFactoryManager ... failed",0,0);
	}
	g_pLog->log_debug("%u,%u,New PacketFactoryManager ... OK",0,0);

	return true;
}

bool LoginServer::InitStaticManager( )
{
	bool bRet = false;

	bRet = g_pServerManager->Init( );
	if(!bRet)
	{
		g_pLog->log_error("%u,%u,g_pServerManager->Init ... Failed",0,0);
		return false;
	}
	g_pLog->log_debug("%u,%u,g_pServerManager->Init ... OK",0,0);


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

	return true;
}

bool LoginServer::DelStaticManager( )
{
	SAFE_DELETE(g_pPlayerPool);
	g_pLog->log_debug("%u,%u,Exit g_pPlayerPool...OK",0,0);

	SAFE_DELETE(g_pPacketFactoryManager);
	g_pLog->log_debug("%u,%u,Exit g_pPacketFactoryManager...OK",0,0);

	SAFE_DELETE(g_pServerManager);
	g_pLog->log_debug("%u,%u,Exit g_pServerManager...OK",0,0);

	return true;
}


int main(int argc, char* argv[])
{

	g_pLog = new Log("Config/loginlogConfig.xml");
	if(!g_pLog)
	{
		Assert(false);
	}


	bool bRet = false;

	bRet = g_LoginServer.Init( );
	if(bRet == false)
	{
		Assert(bRet);
		return 1;
	}

	bRet = g_LoginServer.Loop( );
	if(bRet == false)
	{
		Assert(bRet);
		return 1;
	}

	bRet = g_LoginServer.Exit( );
	if(bRet == false)
	{
		Assert(bRet);
		return 1;
	}

	return 0;
}
