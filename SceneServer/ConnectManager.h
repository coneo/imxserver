/**
 *\version 0.0.1
 *\date 2013-4-17
 *\author
 */

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__


#include "Type.h"
#include "Thread.h"
#include "ScenePlayerManager.h"

class ConnectManager : public Thread
{
public:
	ConnectManager( );
	~ConnectManager( );

	bool 		Init( );

	virtual void 	run( );

	virtual void 	stop( ){ m_Active = false;}

	void 		Quit( );

	bool		IsActive( ){ return m_Active; }

private:

	bool		m_Active;
};

extern ConnectManager * g_pConnectManager;

#endif
