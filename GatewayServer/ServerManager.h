#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "ServerSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "ServerPlayer.h"

#define SCENE_PLAYER_ID 0
#define LOGIN_PLAYER_ID 1
#define BILLING_PLAYER_ID 2

#define MAX_SERVER 24

class ServerManager
{
public:
	ServerManager( );
	~ServerManager( );

	void			CleanUp( );

	bool			Init( );

	bool			Tick( );

	bool			Select( );

	bool			ProcessInputs( );

	bool			ProcessOutputs( );

	bool			ProcessExceptions( );

	bool			ProcessCommands( );

	virtual bool	HeartBeat( );

	bool			ProcessCacheCommands( );

	bool			RecvPacket(Packet* pPacket, PlayerID_t& playerID, UINT& flag);

	bool			ResizeCache( );

public:

	bool			AddServer(SOCKET fd);

	bool			DelServer(SOCKET fd);

	bool			RemoveServer(PlayerID_t id);

	ServerPlayer*	GetServerPlayer(PlayerID_t id);

	bool			ConnectSceneServer( );

	bool			SendQueuePlayerToWorld( );

public:

	bool			IsSceneServerActive( );

	bool			SendPacket(Packet* pPacket, ID_t ServerID, UINT flag=PF_NONE);

private:

	enum
	{
		SELECT_BAK = 0,
		SELECT_USE = 1,
		SELECT_MAX = 2,
	};
	fd_set			m_ReadFDs[SELECT_MAX];
	fd_set			m_WriteFDs[SELECT_MAX];
	fd_set			m_ExceptFDs[SELECT_MAX];

	timeval			m_Timeout[SELECT_MAX];

	SOCKET			m_MinFD;
	SOCKET			m_MaxFD;

	INT				m_nFDSize;

	ServerSocket*	m_pServerSocket;
	SOCKET			m_SocketID;

	ServerPlayer	m_ScenePlayer;

	PlayerID_t		m_pServerHash[MAX_SERVER];

	ASYNC_PACKET* 	m_PacketQue;
	UINT			m_QueSize;
	UINT			m_Head;
	UINT			m_Tail;

public:
	
	TID				m_ThreadID;
};

extern ServerManager*	g_pServerManager;


#endif
