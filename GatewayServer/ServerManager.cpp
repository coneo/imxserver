#include "stdafx.h"

#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "CLAskLogin.h"
#include "Log.h"


ServerManager*	g_pServerManager = NULL;

ServerManager::ServerManager( )
{
	m_pServerSocket = NULL;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE];
	Assert(m_PacketQue);
	m_QueSize = MAX_CACHE_SIZE;
	m_Head = 0;
	m_Tail = 0;

	CleanUp( );
}

ServerManager::~ServerManager( )
{
	SAFE_DELETE(m_pServerSocket);
	SAFE_DELETE_ARRAY(m_PacketQue);
}

void ServerManager::CleanUp( )
{
	FD_ZERO(&m_ReadFDs[SELECT_BAK]);
	FD_ZERO(&m_WriteFDs[SELECT_BAK]);
	FD_ZERO(&m_ExceptFDs[SELECT_BAK]);

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET;

	m_nFDSize = 0;

	m_Head = 0;
	m_Tail = 0;

	m_ScenePlayer.SetPlayerID(SCENE_PLAYER_ID);
}

bool ServerManager::Init( )
{
	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	return true;
}

bool ServerManager::Tick( )
{
	bool ret;

	ret = Select( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,Select Faild",0,0);
	}

	ret = ProcessExceptions( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,ProcessExceptions Faild",0,0);
	}

	ret = ProcessInputs( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,ProcessInputs Faild",0,0);
	}

	ret = ProcessOutputs( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,ProcessOutputs Faild",0,0);
	}

	ret = ProcessCommands( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,ProcessCommands Faild",0,0);
	}

	ret = ProcessCacheCommands( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,ProcessCacheCommands Faild",0,0);
	}

	ret = HeartBeat( );
	if(!ret)
	{
		g_pLog->log_debug("%u,%u,HeartBeat Faild",0,0);
	}

	//ret = SendQueuePlayerToWorld( );
	
	return true;
}

bool ServerManager::Select( )
{
	MySleep(100);
	if(m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET)
	{
		return true;
	}

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]	= m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]	= m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE]	= m_ExceptFDs[SELECT_BAK];

	int ret = SocketAPI::select_ex( (INT)m_MaxFD + 1,
									&m_ReadFDs[SELECT_USE],
									&m_WriteFDs[SELECT_USE],
									&m_ExceptFDs[SELECT_USE],
									&m_Timeout[SELECT_USE] );
	
	if(ret==SOCKET_ERROR)
	{
		g_pLog->log_debug("%u,%u,ServerManager::Select .... Error",0,0);
	}

	return true;
}

bool ServerManager::RemoveServer(PlayerID_t id)
{
	bool ret = false;

	switch (id)
	{
	case SCENE_PLAYER_ID:
		{
			SOCKET s = m_ScenePlayer.GetSocket()->getSOCKET( );
			Assert(s!=INVALID_SOCKET);

			ret = DelServer( s );
			Assert(ret);

			m_ScenePlayer.CleanUp( );

			g_pLog->log_debug("%u,%u,RemoveServer().....Scene....OK",0,0);
		}
		break;
	case LOGIN_PLAYER_ID:
		{
			return true;;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			return true;
		}
		break;
	default:
		{
			return false;
		}
	}
	return ret;
}

ServerPlayer* ServerManager::GetServerPlayer(PlayerID_t id)
{
	switch (id)
	{
	case SCENE_PLAYER_ID:
		{
			return &m_ScenePlayer;
		}
		break;
	case LOGIN_PLAYER_ID:
		{
			return NULL;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			return NULL;
		}
		break;
	default:
		{
			return NULL;
		}
	}

	return NULL;

}

bool ServerManager::ProcessInputs( )
{
	bool ret = false;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	for(int i=SCENE_PLAYER_ID; i<=BILLING_PLAYER_ID; i++)
	{
		ServerPlayer*	pPlayer = GetServerPlayer(i);
		if(!pPlayer) continue;

		if(pPlayer->IsValid())
		{
			SOCKET s = pPlayer->GetSocket()->getSOCKET();
			if(FD_ISSET( s, &m_ReadFDs[SELECT_USE]) )
			{
				if(pPlayer->GetSocket()->isSockError() )
				{
					RemoveServer(i);
				}
				else
				{
					ret = pPlayer->ProcessInput( );
					if(!ret)
					{
						RemoveServer( i );
					}
				}
			}
		}
	}

	return true;
}


bool ServerManager::ProcessOutputs( )
{
	bool ret = false;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	for(int i=SCENE_PLAYER_ID; i<=BILLING_PLAYER_ID; i++)
	{
		ServerPlayer*	pPlayer = GetServerPlayer(i);
		if(!pPlayer) continue;

		if(pPlayer->IsValid())
		{
			SOCKET s = pPlayer->GetSocket()->getSOCKET();
			if(FD_ISSET( s, &m_WriteFDs[SELECT_USE]) )
			{
				if(pPlayer->GetSocket()->isSockError() )
				{
					RemoveServer(i);
				}
				else
				{
					ret = pPlayer->ProcessOutput( );
					if(!ret)
					{
						RemoveServer( i );
					}
				}
			}
		}
	}

	return true;
}


bool ServerManager::ProcessExceptions( )
{
	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	for(int i=SCENE_PLAYER_ID; i<=BILLING_PLAYER_ID; i++)
	{
		ServerPlayer*	pPlayer = GetServerPlayer(i);
		if(!pPlayer) continue;

		if(pPlayer->IsValid())
		{
			SOCKET s = pPlayer->GetSocket()->getSOCKET();
			if(FD_ISSET( s, &m_ExceptFDs[SELECT_USE]) )
			{
				g_pLog->log_debug("%u,%u,ProcessException......",pPlayer->PlayerID(),0);
				RemoveServer(i);
			}
		}
	}

	return true;
}


bool ServerManager::ProcessCommands( )
{
	bool ret = false;

	if(m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{
		return true;
	}

	for(int i=SCENE_PLAYER_ID; i<=BILLING_PLAYER_ID; i++)
	{
		ServerPlayer*	pPlayer = GetServerPlayer(i);
		if(!pPlayer) continue;

		if(pPlayer->IsValid())
		{
			if(pPlayer->GetSocket()->isSockError() )
			{
				RemoveServer(i);
			}
			else
			{
				ret = pPlayer->ProcessCommand(false);
				if(!ret)
				{
					RemoveServer( i );
				}
			}
		}
	}

	return true;
}

bool ServerManager::AddServer(SOCKET fd)
{
	if(fd==INVALID_SOCKET)
		return false;

	if(m_nFDSize >= FD_SETSIZE)
	{
		g_pLog->log_debug("%u,%u,m_nFDSize >= FD_SETSIZE....",0,0);
		return false;
	}

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd, m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd, m_MaxFD));

	FD_SET(fd, &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd, &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd, &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++;

	return true;
}


//hava problem
bool ServerManager::DelServer(SOCKET fd)
{
	if(m_MinFD == INVALID_SOCKET) return false;
	if(m_MaxFD == INVALID_SOCKET) return false;
	if(fd == INVALID_SOCKET) return false;

	m_MinFD = m_MaxFD = INVALID_SOCKET;
	FD_CLR(fd, &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd, &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd, &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd, &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd, &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd, &m_ExceptFDs[SELECT_USE]);

	m_nFDSize--;
	Assert(m_nFDSize>=0);

	return true;
}

bool ServerManager::HeartBeat( )
{
	bool ret = false;
	if(IsSceneServerActive())
	{
		return true;
	}

	if(!IsSceneServerActive())
	{
		ret = ConnectSceneServer( );
		if(ret == false)
		{
			MySleep(2000);
		}
	}
	return true;
}

bool ServerManager::IsSceneServerActive()
{
	return m_ScenePlayer.IsValid();
}

bool ServerManager::ConnectSceneServer( )
{
	bool ret = false;
	//SSConnect* msg = NULL;
	Socket* pSocket = m_ScenePlayer.GetSocket();
	//_SERVER_DATA* pSceneData = &(g_Config.m_ServerInfo.m_scene);
	
	ret = pSocket->create( );
	if(!ret)
	{
		g_pLog->log_error("%u,%u,Connect to SceneServer ... Failed",0,0);
		m_ScenePlayer.CleanUp( );
		return false;
	}
	
	const char ip[16] = "192.168.192.129";
	UINT port = 8787;
	//端口ip可配
	ret = pSocket->connect(ip,port);
	if(!ret)
	{
		g_pLog->log_error("%u,%u,Connect to SceneServer ... Failed",0,0);
		m_ScenePlayer.CleanUp( );
		return false;
	}

	ret = pSocket->setNonBlocking( );
	if(!ret)
	{
		g_pLog->log_error("%u,%u,Connect to SceneServer ... Failed",0,0);
		m_ScenePlayer.CleanUp( );
		return false;
	}

	ret = pSocket->setLinger(0);
	if(!ret)	
	{
		g_pLog->log_error("%u,%u,Connect to SceneServer ... Failed",0,0);
		m_ScenePlayer.CleanUp( );
		return false;
	}

	ret = AddServer(pSocket->getSOCKET());
	if(!ret)
	{
		g_pLog->log_error("%u,%u,Connect to SceneServer ... Failed",0,0);
		m_ScenePlayer.CleanUp( );
		return false;
	}
	
	//msg = new SSConnect;
	//ret = m_ScenePlayer.SendPacket(msg);
	
	/*CLAskLogin  msg;
	msg.SetAccount("hxq");
	msg.SetPassWord("123456");
	ret = m_ScenePlayer.SendPacket(&msg);
	g_pLog->log_debug("%u,%u,SendPacket(CLAskLogin) to SceneServer ... OK",0,0);*/
	
	g_pLog->log_debug("%u,%u,Connect to SceneServer ... OK",0,0);
	return true;
}

bool ServerManager::SendPacket(Packet* pPacket, ID_t ServerID, UINT flag)
{
	return true;
}

bool ServerManager::RecvPacket(Packet* pPacket, PlayerID_t& playerID, UINT& flag)
{
	return true;
}

bool ServerManager::ProcessCacheCommands( )
{
	return true;
}

bool ServerManager::ResizeCache( )
{
	return true;
}
