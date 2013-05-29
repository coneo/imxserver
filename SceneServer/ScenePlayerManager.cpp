#include "stdafx.h"

#include "ScenePlayerManager.h"
#include "PlayerPool.h"
#include "CLAskLogin.h"
#include "Log.h"

#define ACCEPT_ONESTEP 50	/*一下子能够接受的连接数*/

ScenePlayerManager*	 g_pScenePlayerManager = NULL;

ScenePlayerManager::ScenePlayerManager( )
{
	FD_ZERO( &m_ReadFDs[SELECT_BAK] );
	FD_ZERO( &m_WriteFDs[SELECT_BAK] );
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] );

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET;

	m_nFDSize = 0;
}

ScenePlayerManager::~ScenePlayerManager( )
{
	SAFE_DELETE(m_pServerSocket);
}

bool ScenePlayerManager::Init( )
{
	INT port = 8787;  /*服务器监听端口，应该可配的*/

	m_pServerSocket = new ServerSocket( port );
	Assert( m_pServerSocket );

	m_pServerSocket->setNonBlocking( );

	m_SocketID = m_pServerSocket->getSOCKET( );
	Assert(m_SocketID != INVALID_SOCKET);

	FD_SET(m_SocketID, &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID, &m_WriteFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	//time为零，表示不等待
	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	return true;
}

bool ScenePlayerManager::Select( )
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
	Assert(iRet!=SOCKET_ERROR);

	return true;
}

bool ScenePlayerManager::ProcessInputs( )
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

		ScenePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
		//assert	
		if(!pPlayer)
			continue;

		SOCKET s = pPlayer->GetSocket()->getSOCKET();
		if(s == m_SocketID) /*排除自己*/
			continue;

		if(FD_ISSET( s, &m_ReadFDs[SELECT_USE]) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{
				std::cout<<"sock Error\n";
				RemovePlayer( pPlayer );
			}
			else
			{
				ret = pPlayer->ProcessInput( );
				if( !ret )
				{
					std::cout<<"ProcessInputs Error\n";
					RemovePlayer( pPlayer );
				}
			}
		}
			
	}


	return true;
}

bool ScenePlayerManager::ProcessOutputs( )
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

		ScenePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
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

bool ScenePlayerManager::ProcessExceptions( )
{
 	return true;
}

/**
 *\处理接收的消息
 *\return 是否处理成功
 */
bool ScenePlayerManager::ProcessCommands( )
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

		ScenePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
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

bool ScenePlayerManager::AcceptNewConnection( )
{
	UINT nPlayerCount = GetPlayerNumber( );
	g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection",0,nPlayerCount);

	bool ret = false;

	ScenePlayer*	client = g_pPlayerPool->NewPlayer( );
	if( client==NULL ) return false;
	g_pLog->log_info("%u,%u,Scene new Player ...",0,0);

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
		client->CleanUp( );
		g_pPlayerPool->DelPlayer( client->PlayerID() );
		g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection exception-------end",0,0);
	}
	ret = (INT)client->GetSocket()->setNonBlocking( );
	if(!ret)
	{
		Assert(false);
		client->CleanUp( );
		g_pPlayerPool->DelPlayer( client->PlayerID() );
		g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection exception-------end",0,0);
	}
	g_pLog->log_info("%u,%u,setNonBlocking ...",0,0);
	if(client->GetSocket()->getSockError() )
	{
		Assert(false);
		client->CleanUp( );
		g_pPlayerPool->DelPlayer( client->PlayerID() );
		g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection exception-------end",0,0);
	}
	ret = client->GetSocket()->setLinger( 0 );
	if(!ret)
	{
		Assert(false);
		client->CleanUp( );
		g_pPlayerPool->DelPlayer( client->PlayerID() );
		g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection exception-------end",0,0);
	}

	client->Init( );
	//client->SetPlayerStatus( PS_LOGIN_CONNECT);
	
	ret = AddPlayer( client );
	if(!ret)
	{
		Assert(false);
		client->CleanUp( );
		g_pPlayerPool->DelPlayer( client->PlayerID() );
		g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection exception-------end",0,0);
	}

	g_pLog->log_debug("%u,%u,ScenePlayerManager::AcceptNewConnection-------end",0,0);

	/*char account[MAX_ACCOUNT+1]="hxq";
	char password[MAX_PASSWORD+1]="hxq";
	CLAskLogin msg;
	msg.SetAccount(account);
	msg.SetPassWord(password);
	msg.SetVersion(22);
	ret = client->SendPacket(&msg);
	g_pLog->log_debug("%u,%u,SendPacket to Gateway ... packID:%u",0,0,msg.GetPacketID());*/
	

	return true;
}

bool ScenePlayerManager::HeartBeat( )
{
	return true;
}

bool ScenePlayerManager::AddPlayer( Player* pPlayer )
{
	if(m_nFDSize >= FD_SETSIZE)
	{
		//assert
		std::cout<<"超出能够检测的网络句柄最大数"<<std::endl;
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

	g_pLog->log_debug("%u,%u,ScenePlayerManager::AddPlayer ... OK",pPlayer->PlayerID(),0);

	return true;
}

bool ScenePlayerManager::DelPlayer( PlayerID_t pid )
{
	Assert( pid != INVALID_ID );
	ScenePlayer*	pScenePlayer = g_pPlayerPool->GetPlayer(pid);
	Assert( pScenePlayer );

	SOCKET fd = pScenePlayer->GetSocket()->getSOCKET();

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

			ScenePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
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

			ScenePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]);
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
	g_pLog->log_debug("%u,%u,ScenePlayerManager::DelPlayer ... OK",pid,0);

	return true;
}

bool ScenePlayerManager::RemovePlayer( Player* pPlayer )
{
 	bool ret = false;

	ret = DelPlayer(pPlayer->PlayerID());
	Assert(ret);

	((ScenePlayer*)pPlayer)->FreeOwn( );

	return ret;
}

void ScenePlayerManager::RemoveAllPlayers( )
{
}
