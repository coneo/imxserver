#include "stdafx.h"
#include "ServerManager.h"
#include "Log.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"

#define ACCEPT_ONESTEP 50


ServerManager* g_pServerManager = NULL ;

ServerManager::ServerManager( )
{
	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_iFDSize = 0 ;
	
	SetActive(TRUE) ;
}

ServerManager::~ServerManager( )
{
	SAFE_DELETE( m_pServerSocket ) ;
}

BOOL ServerManager::Init( )
{
	INT port = 9594;

	m_pServerSocket = new ServerSocket( port ) ;
	Assert( m_pServerSocket ) ;

	m_pServerSocket->setNonBlocking() ;

	m_SocketID = m_pServerSocket->getSOCKET() ;
	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	//m_ThreadID = MyGetCurrentThreadID( ) ;

	for( int i=0; i<OVER_MAX_SERVER; i++ )
	{
		m_aServerHash[i] = INVALID_ID ;
	}


	return TRUE ;
}

BOOL ServerManager::Select( )
{
	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	MySleep(100) ;

	int iRet = SocketAPI::select_ex(	(int)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
	Assert( iRet!=SOCKET_ERROR ) ;

	return TRUE ;
}

BOOL ServerManager::RemovePlayer( Player* pPlayer )
{
	BOOL ret = FALSE ;

	//ID_t	ServerID		=	((ServerPlayer*)pPlayer)->GetServerData()->m_ServerID ;
	//BOOL	bUseShareMem	=   FALSE;	
	
	ret = DelPlayer( pPlayer ) ;
	Assert( ret ) ;

	((ServerPlayer*)pPlayer)->FreeOwn( ) ;

	

	return ret ;
}

BOOL ServerManager::ProcessInputs( )
{
	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	if( FD_ISSET(m_SocketID,&m_ReadFDs[SELECT_USE]) )
	{
		for( int i=0; i<ACCEPT_ONESTEP; i++ )
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
			{//��������
				ret = pPlayer->ProcessInput( ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}


	return TRUE ;

}

BOOL ServerManager::ProcessOutputs( )
{
	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
			{//��������
				ret = pPlayer->ProcessOutput( ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}

	return TRUE ;

}

BOOL ServerManager::ProcessExceptions( )
{

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		//ĳ����ҶϿ���������

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
		{//��������������⣬�ѡ�����
			Assert( FALSE ) ;
			continue ;
		}

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			RemovePlayer( pPlayer ) ;
		}
	}



	return TRUE ;

}

BOOL ServerManager::ProcessCommands( )
{

	BOOL ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			RemovePlayer( pPlayer ) ;
		}
		else
		{//��������
			ret = pPlayer->ProcessCommand( FALSE ) ;
			if( !ret )
			{
				RemovePlayer( pPlayer ) ;
			}
		}
	}


	return TRUE ;

}

BOOL ServerManager::AcceptNewConnection( )
{

	int iStep = 0 ;
	BOOL ret = FALSE ;

	ServerPlayer* client = g_pPlayerPool->NewPlayer() ;
//	Assert( client ) ;
	if( client==NULL )
		return FALSE ;

	iStep = 5 ;
	client->CleanUp( ) ;

	int fd = INVALID_SOCKET ;

	iStep = 10 ;
		ret = m_pServerSocket->accept( client->GetSocket() ) ;
		if( !ret )
		{
			iStep = 15 ;
			goto EXCEPTION ;
		}
		iStep = 30 ;
		fd = (int)client->GetSocket()->getSOCKET();
		if( fd == INVALID_SOCKET )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 40 ;
		ret = client->GetSocket()->setNonBlocking() ;
		if( !ret )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 50 ;
		if( client->GetSocket()->getSockError() )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 60 ;
		ret = client->GetSocket()->setLinger(0) ;
		if( !ret )
		{	
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 70 ;
		client->Init( ) ;
		//client->SetPlayerStatus( PS_WORLD_CONNECT ) ;

		iStep = 80 ;
		ret = AddPlayer( client ) ;
		if( !ret )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

	g_pLog->log_debug("%u,%u,AcceptNewConnectio ... OK",0,0);

	return TRUE ;


EXCEPTION:
	client->CleanUp() ;
	g_pPlayerPool->DelPlayer( client->PlayerID() ) ;

	return FALSE ;

}

BOOL ServerManager::AddPlayer( Player* pPlayer )
{

	if( m_iFDSize>=FD_SETSIZE )
	{//�Ѿ������ܹ������������������
		Assert(FALSE) ;
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		Assert(FALSE) ;
		return FALSE ;
	}
	m_aServerHash[pPlayer->PlayerManagerID()] = pPlayer->PlayerID(); 

	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	Assert( fd != INVALID_SOCKET ) ;

	m_MinFD = min(fd , m_MinFD);
	m_MaxFD = max(fd , m_MaxFD);

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_iFDSize++ ;

	return TRUE ;

}

BOOL ServerManager::DelPlayer( Player* pPlayer )
{

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	Assert( pServerPlayer ) ;

	SOCKET fd = pServerPlayer->GetSocket()->getSOCKET() ;

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	if( fd==INVALID_SOCKET ) 
	{
		Assert(FALSE) ;
	}

	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		UINT nPlayerCount = GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp < s )
			{
				s = temp ;
			}
		}

		if( m_MinFD == m_MaxFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			if( s > m_SocketID )
			{
				m_MinFD = m_SocketID ;
			}
			else
			{
				m_MinFD = s ;
			}
		}
	} 
	else if( fd==m_MaxFD ) 
	{
		SOCKET s = m_MinFD ;
		UINT nPlayerCount = GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp > s )
			{
				s = temp ;
			}
		}

		if( m_MaxFD == m_MinFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			if( s < m_SocketID )
			{
				m_MaxFD = m_SocketID ;
			}
			else
			{
				m_MaxFD = s ;
			}
		}
	}

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_iFDSize-- ;
	Assert( m_iFDSize>=0 ) ;

	m_aServerHash[pPlayer->PlayerManagerID()] = INVALID_ID; 

	PlayerManager::RemovePlayer( pPlayer->PlayerID() ) ;

	return TRUE ;

}

BOOL ServerManager::HeartBeat( )
{
/*
	BOOL ret ;

	DWORD dwTime = g_pTimeManager->CurrentTime() ;

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}

		ret = pPlayer->HeartBeat( dwTime ) ;
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			ret = RemovePlayer( pPlayer ) ;
			Assert( ret ) ;
		}
	}
*/

	return TRUE ;

}

void ServerManager::RemoveAllPlayer( )
{

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[0] == INVALID_ID )
			break ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[0]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			break ;
		}
	
		RemovePlayer( pPlayer ) ;
	}

}

void ServerManager::Loop( )
{

while( IsActive() )
{
	BOOL ret = FALSE ;
	//UINT uTime = g_pTimeManager->CurrentTime() ;

		ret = Select( ) ;
		Assert( ret ) ;

		ret = ProcessExceptions( ) ;
		Assert( ret ) ;

		ret = ProcessInputs( ) ;
		Assert( ret ) ;

		ret = ProcessOutputs( ) ;
		Assert( ret ) ;

		ret = ProcessCommands( ) ;
		Assert( ret ) ;

		ret = HeartBeat( ) ;
		Assert( ret ) ;
};

}

ServerPlayer* ServerManager::GetServerPlayer( ID_t ServerID )
{

	Assert( ServerID>=0 && ServerID<OVER_MAX_SERVER ) ;
	PlayerID_t pid = m_aServerHash[ServerID] ;

	ServerPlayer* pServerPlayer = g_pPlayerPool->GetPlayer( pid ) ;
	Assert( pServerPlayer ) ;
	
	return pServerPlayer ;

}

VOID ServerManager::BroadCast( Packet* pPacket )
{
/*
	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}

		pPlayer->SendPacket( pPacket ) ;
	}
*/
}


