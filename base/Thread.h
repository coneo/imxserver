/**
 *\Version:0.0.1
 *\Date:2013-3-28
 *\Author: 
 */

#ifndef __THREAD_H__
#define __THREAD_H__


#include "Type.h"

class Thread
{
public:
	enum ThreadStatus
	{
		READY,
		RUNNING,
		EXITING,
		EXIT
	};

public:
	Thread( );
	~Thread( );

	void start( );
	
	virtual void stop( );
	
	void exit(void *retval = NULL);
	
	virtual void run( );

public:
	pthread_t getTID( ){ return m_TID; }

	ThreadStatus getStatus( ){ return m_Status; }
	void setStatus( ThreadStatus status){ m_Status = status; }

private:
	pthread_t	m_TID;		/*线程ID*/

	ThreadStatus  m_Status;	 /*线程状态*/
};


extern unsigned int g_QuitThreadCount;
/**
 *\param derivedThread:线程函数
 */
void * MyThreadProcess( void * derivedThread );


#endif
