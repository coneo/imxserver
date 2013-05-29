/**
 *\Version:0.0.1
 *\Date:2013-3-29
 *\Author: 
 */

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__

#include "Type.h"
#include "Thread.h"
#include "GatewayPlayerManager.h"

class ConnectManager : public Thread
{
public:
	ConnectManager( );
	~ConnectManager( );

	bool		Init( );
	
	virtual void	run( );

	virtual void 	stop( ){ m_Active = false; };

	void 		Quit( );

	bool 		IsActive( ){ return m_Active; };
private:
	bool		m_Active;	 /*活动标志*/

};

extern ConnectManager * g_pConnectManager;

#endif
