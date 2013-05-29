
#ifndef __GATEWAY_PLAYER_MANAGER_H__
#define __GATEWAY_PLAYER_MANAGER_H__


#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"

class GatewayPlayerManager : public PlayerManager
{
public:
	GatewayPlayerManager( );
	~GatewayPlayerManager( );

	bool		Init( );
	bool		Select( );
	bool		ProcessInputs( );
	bool		ProcessOutputs( );
	bool		ProcessExceptions( );
	bool		ProcessCommands( );
	bool		AcceptNewConnection( );
	bool		HeartBeat( );

public:
	bool		AddPlayer( Player* pPlayer );
	bool		DelPlayer( PlayerID_t pid);
	bool		RemovePlayer( Player* pPlayer );
	void		RemoveAllPlayers( );

private:
	ServerSocket*		m_pServerSocket; 	/*服务器监听套接字*/
	SOCKET				m_SocketID;			/*监听套接字ID*/

	enum
	{
		SELECT_BAK = 0,		/*当前拥有的完整句柄数据*/
		SELECT_USE = 1,		/*用于select使用的句柄数据*/
		SELECT_MAX = 2,
	};
	fd_set			m_ReadFDs[SELECT_MAX];	
	fd_set			m_WriteFDs[SELECT_MAX];
	fd_set			m_ExceptFDs[SELECT_MAX];

	timeval			m_Timeout[SELECT_MAX];

	SOCKET			m_MinFD;
	SOCKET			m_MaxFD;

	int				m_nFDSize;
public:
	TID		m_ThreadID;

};

extern GatewayPlayerManager*  g_pGatewayPlayerManager ;

#endif
