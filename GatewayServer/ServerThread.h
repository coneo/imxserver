
#ifndef __SERVER_THREAD_H__
#define __SERVER_THREAD_H__

#include "Type.h"
#include "Thread.h"

class ServerThread : public Thread
{
public:
	ServerThread( );
	~ServerThread( );

	virtual void	run( );

	virtual void 	stop( ){ m_Active = false;};

	bool	IsActive( ){ return m_Active;};

private:
	bool	m_Active;

};
#endif
