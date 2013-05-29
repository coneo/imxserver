#include "stdafx.h"

#include "ServerThread.h"
#include "ServerManager.h"
#include "Assertx.h"
#include "Log.h"

ServerThread::ServerThread( )
{
	m_Active = true;
}

ServerThread::~ServerThread( )
{
}

void ServerThread::run( )
{
	g_pLog->log_debug("%u,%u,ServerThread::run(),ThreadID %d",0,0,getTID());
	g_pServerManager->m_ThreadID = getTID( );

	while(IsActive() )
	{
		if(g_pServerManager)
		{
			MySleep(100);
			bool ret = g_pServerManager->Tick( );
			Assert( ret );
		}
	}
}
