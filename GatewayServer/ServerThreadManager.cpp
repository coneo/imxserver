#include "stdafx.h"

#include "ServerThreadManager.h"
#include "Log.h"

ServerThreadManager*	g_pServerThreadManager=NULL;

ServerThreadManager::ServerThreadManager( )
{
	m_pServerThread = new ServerThread;
	if(!m_pServerThread)
	{
		g_pLog->log_debug("%u,%u,Create m_pServerThread Failed",0,0);
	}

	m_nThreads = 0;
}

ServerThreadManager::~ServerThreadManager( )
{
	SAFE_DELETE(m_pServerThread);
}

bool ServerThreadManager::Init( )
{
	if(m_pServerThread->IsActive() )
	{
		m_nThreads++;
	}
	return true;
}

bool ServerThreadManager::Start( )
{
	m_pServerThread->start( );

	return true;
}

bool ServerThreadManager::Stop( )
{
	if(m_pServerThread)
	{
		m_pServerThread->stop( );
	}
	return true;
}
