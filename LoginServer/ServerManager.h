
#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"
#include "Thread.h"
//#include "Config.h"


class ServerPlayer ;

class ServerManager : public PlayerManager
{
public :
	ServerManager() ;
	~ServerManager() ;

	BOOL				Init( ) ;
	BOOL				Select( ) ;
	BOOL				ProcessInputs( ) ;
	BOOL				ProcessOutputs( ) ;
	BOOL				ProcessExceptions( ) ;
	BOOL				ProcessCommands( ) ;
	BOOL				AcceptNewConnection( ) ;
	virtual BOOL		HeartBeat( ) ;

	void				Loop( ) ;

	BOOL				IsActive( ){ return m_bActive ; } ;
	void				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;

public :
	BOOL				AddPlayer( Player* pPlayer ) ;
	BOOL				DelPlayer( Player* pPlayer ) ;

	BOOL				RemovePlayer( Player* pPlayer ) ;
	void				RemoveAllPlayer( ) ;

	ServerPlayer*		GetServerPlayer( ID_t ServerID ) ;

	void				BroadCast( Packet* pPacket ) ;


private :
	ServerSocket*		m_pServerSocket ;
	SOCKET				m_SocketID ;

	//
	enum{
		SELECT_BAK = 0,
		SELECT_USE = 1,
		SELECT_MAX = 2,
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET		m_MaxFD;

	int			m_iFDSize ;

	BOOL		m_bActive ;


public :
	TID			m_ThreadID ;
	PlayerID_t	m_aServerHash[OVER_MAX_SERVER] ;	

};

extern ServerManager* g_pServerManager ;

#endif
