#include "stdafx.h"

#include "GatewayPlayerManager.h"
#include "PlayerPool.h"
#include "Log.h"


#define ACCEPT_ONESTEP 50	/*一下子能够处理的连接数*/

GatewayPlayerManager*  g_pGatewayPlayerManager = NULL;

GatewayPlayerManager::GatewayPlayerManager( )
{
	FD_ZERO( &m_ReadFDs[SELECT_BAK] );
	FD_ZERO( &m_WriteFDs[SELECT_BAK] );
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] );

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET;

	m_nFDSize = 0;
}

GatewayPlayerManager::~GatewayPlayerManager( )
{
	SAFE_DELETE( m_pServerSocket );
}

bool GatewayPlayerManager::Init( )
{
	INT port = 9090;	/*服务器监听端口*/

	m_pServerSocket = new ServerSocket( port );
	Assert(m_pServerSocket);
	
	m_pServerSocket->setNonBlocking( );

	m_SocketID = m_pServerSocket->getSOCKET( );
	if(m_SocketID == INVALID_SOCKET) return false;
	Assert(m_SocketID);
	
	FD_SET(m_SocketID, &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID, &m_WriteFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	//time为零，表示不等待
	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	return true;
}

bool GatewayPlayerManager::Select( )
{
	m_Timeout[SELECT_USE].tv_sec	= m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec	= m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]		= m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]		= m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE]		= m_ExceptFDs[SELECT_BAK];


	MySleep(100);

	INT	iRet = SocketAPI::select_ex( (INT)m_MaxFD+1,
									&m_ReadFDs[SELECT_USE],
									&m_WriteFDs[SELECT_USE],
									&m_ExceptFDs[SELECT_USE],
									&m_Timeout[SELECT_USE] );
	//assert iRet
	if( iRet == SOCKET_ERROR ) return false;

	return true;
}

bool GatewayPlayerManager::ProcessInputs( )
{
	bool ret = false;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET )
		return true;

	if( FD_ISSET( m_SocketID, &m_ReadFDs[SELECT_USE]) )
	{
		for(INT i=0; i<ACCEPT_ONESTEP; i++)
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	//处理数据流入
	UINT nPlayerCount = GetPlayerNumber();
	for( UINT i=0; i<nPlayerCount; i++)
	{
		if(m_pPlayers[i] == INVALID_ID)
			continue;

		GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
		if(!pPlayer)
			continue;

		SOCKET s = pPlayer->GetSocket()->getSOCKET();
		if(s == m_SocketID) /*排除自己*/
			continue;

		if(FD_ISSET( s, &m_ReadFDs[SELECT_USE]) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{
				RemovePlayer( pPlayer );
			}
			else
			{
				ret = pPlayer->ProcessInput( );
				if( !ret )
				{
					RemovePlayer( pPlayer );
				}
			}
		}
			
	}


	return true;
}

bool GatewayPlayerManager::ProcessOutputs( )
{
	bool ret = false;
	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	UINT nPlayerCount = GetPlayerNumber( );
	for(UINT i=0; i<nPlayerCount; i++)
	{
		if(m_pPlayers[i]==INVALID_ID)
			continue;

		GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
		Assert(pPlayer);

		SOCKET s = pPlayer->GetSocket( )->getSOCKET( );
		if(s == m_SocketID)
			continue;

		if(FD_ISSET( s, &m_WriteFDs[SELECT_USE]) )
		{
			if(pPlayer->GetSocket()->isSockError() )
			{
				RemovePlayer(pPlayer);
			}
			else
			{
				ret = pPlayer->ProcessOutput( );
				if(!ret)
				{
					RemovePlayer(pPlayer);
				}
			}
		}
	}
	return true;
}

bool GatewayPlayerManager::ProcessExceptions( )
{
	return true;
}

/**
 *\处理接收的消息
 *\return 是否处理成功
 */
bool GatewayPlayerManager::ProcessCommands( )
{
	bool ret;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	UINT nPlayerCount = GetPlayerNumber( );
	for(UINT i=0; i<nPlayerCount; i++)
	{
		if(m_pPlayers[i] == INVALID_ID)
			continue;

		GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
		Assert(pPlayer);

		SOCKET s = pPlayer->GetSocket()->getSOCKET( );
		if(s == m_SocketID)
			continue;

		if(pPlayer->GetSocket()->isSockError())
		{
			RemovePlayer(pPlayer);
		}
		else
		{
			ret = pPlayer->ProcessCommand( false );
			if( !ret )
			{
				RemovePlayer(pPlayer);
			}
		}
	}

	return true;
}

bool GatewayPlayerManager::AcceptNewConnection( )
{
	bool ret = false;

	std::cout<<"enter AcceptNewConnection ... "<<std::endl;

	GatewayPlayer*	client = g_pPlayerPool->NewPlayer( );
	if( client==NULL ) return false;
	g_pLog->log_info("%u,%u,new Player ...",0,0);

	client->CleanUp( );

	INT fd = INVALID_SOCKET;

	ret = m_pServerSocket->accept( client->GetSocket() );
	if( !ret )
	{
		client->CleanUp( );
		g_pPlayerPool->DelPlayer(client->PlayerID() );
		return false;
	}
	g_pLog->log_info("%u,%u,accept ...",0,0);

	fd = (INT)client->GetSocket()->getSOCKET();
	if( fd==INVALID_SOCKET )
	{
		Assert (false);
		goto EXCEPTION;
	}
	ret = (INT)client->GetSocket()->setNonBlocking( );
	if(!ret)
	{
		Assert(false);
		goto EXCEPTION;
	}
	if(client->GetSocket()->getSockError() )
	{
		Assert(false);
		goto EXCEPTION;
	}
	ret = client->GetSocket()->setLinger( 0 );
	if(!ret)
	{
		Assert(false);
		goto EXCEPTION;
	}

	client->Init( );
	//client->SetPlayerStatus( PS_LOGIN_CONNECT);
	
	ret = AddPlayer( client );
	if(!ret)
	{
		Assert(false);
		goto EXCEPTION;
	}

	g_pLog->log_debug("%u,%u,AcceptNewConnection",0,0);

	//测试发包
	return true;

EXCEPTION:

	client->CleanUp( );
	g_pPlayerPool->DelPlayer( client->PlayerID() );

	return false;
}

bool GatewayPlayerManager::HeartBeat( )
{
	return true;
}

bool GatewayPlayerManager::AddPlayer( Player* pPlayer )
{
	if(m_nFDSize >= FD_SETSIZE)
	{
		Assert(false);
		g_pLog->log_debug("%u,%u,超出能够检测的网络句柄最大数",0,0);
		return false;
	}

	bool ret = PlayerManager::AddPlayer( pPlayer );
	if( !ret )
	{
		Assert(false);
		return false;
	}

	//GatewayPlayer* pGatewayPlayer = (GatewayPlayer*)pPlayer;
	//pGatewayPlayer->SetSceneID( INVALID_ID);

	SOCKET fd = pPlayer->GetSocket()->getSOCKET( );
	if(fd == INVALID_SOCKET) return false;

	m_MinFD = std::min(fd, m_MinFD);
	m_MaxFD = std::max(fd, m_MaxFD);

	FD_SET(fd, &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd, &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd, &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++;

	return true;
}

bool GatewayPlayerManager::DelPlayer( PlayerID_t pid )
{
	Assert( pid != INVALID_ID );
	GatewayPlayer*	pGatewayPlayer = g_pPlayerPool->GetPlayer(pid);
	Assert( pGatewayPlayer );

	SOCKET fd = pGatewayPlayer->GetSocket()->getSOCKET();

	Assert( m_MinFD!=INVALID_ID);
	Assert( m_MaxFD!=INVALID_ID);
	if(fd==INVALID_SOCKET)
	{
		Assert(false);
	}

	if(fd==m_MinFD)
	{
		SOCKET s = m_MaxFD;
		UINT nPlayerCount = GetPlayerNumber( );
		for(UINT i=0; i<nPlayerCount; i++)
		{
			if(m_pPlayers[i]==INVALID_ID)
				continue;

			GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
			Assert(pPlayer);
			if(pPlayer==NULL)
				continue;

			SOCKET temp = pPlayer->GetSocket()->getSOCKET( );
			if(temp==fd)
				continue;
			if(temp==INVALID_SOCKET)
				continue;

			if(temp < s)
			{
				s = temp;
			}
		}

		if(m_MinFD == m_MaxFD)
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET;
		}
		else
		{
			if(s > m_SocketID)
			{
				m_MinFD = m_SocketID;
			}
			else
			{
				m_MinFD = s;
			}
		}
	}
	else if(fd == m_MaxFD)
	{
		SOCKET s = m_MinFD;
		UINT nPlayerCount = GetPlayerNumber( );
		for(UINT i=0; i<nPlayerCount; i++)
		{
			if(m_pPlayers[i]==INVALID_ID)
				continue;

			GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
			Assert(pPlayer);
			if(pPlayer==NULL)
				continue;

			SOCKET temp = pPlayer->GetSocket()->getSOCKET();
			if(temp == fd)
				continue;
			if(temp == INVALID_SOCKET)
				continue;

			if(temp > s)
			{
				s = temp;
			}
		}

		if(m_MaxFD == m_MinFD)
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET;
		}
		else
		{
			if(s < m_SocketID)
			{
				m_MaxFD = m_SocketID;
			}
			else
			{
				m_MaxFD = s;
			}
		}
	}

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize--;
	Assert(m_nFDSize >= 0);

	PlayerManager::RemovePlayer( pid );

	return true;
}

bool GatewayPlayerManager::RemovePlayer( Player* pPlayer)
{
	bool ret = false;

	ret = DelPlayer(pPlayer->PlayerID());
	Assert(ret);

	((GatewayPlayer*)pPlayer)->FreeOwn( );

	g_pLog->log_debug("%u,%u,Remove Player", pPlayer->PlayerID(),0);
	
	return ret;
}

void GatewayPlayerManager::RemoveAllPlayers( )
{
}
