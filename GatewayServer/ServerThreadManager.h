

#ifndef __SERVER_THREAD_MANAGER_H__
#define __SERVER_THREAD_MANAGER_H__

#include "Type.h"
#include "ServerThread.h"


class ServerThreadManager
{
public:
	ServerThreadManager( );
	~ServerThreadManager( );

	bool		Init( );
	bool		Start( );
	bool		Stop( );

	ServerThread*	GetServerThread( )
	{
		return m_pServerThread;
	};

	UINT			GetTotalThreads(){ return m_nThreads;};

protected:
	ServerThread*	m_pServerThread;
	UINT			m_nThreads;

};
extern ServerThreadManager*		g_pServerThreadManager;



#endif
