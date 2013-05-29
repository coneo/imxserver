#include "stdafx.h"
/**
 *\Version:0.0.1
 *\Date:2013-3-28
 *\Author: 
 */

#include "Thread.h"

unsigned int g_QuitThreadCount = 0;  /*线程计数器*/

Thread::Thread( )
{
	m_TID		= 0;
	m_Status	= Thread::READY;
}

Thread::~Thread( )
{

}

void Thread::start( )
{
	if(m_Status != Thread::READY)
		return;

	pthread_create( &m_TID, NULL, MyThreadProcess, this);
}

void Thread::stop( )
{
	
}

void Thread::exit( void *retval)
{
	pthread_exit( retval );
}

void * MyThreadProcess( void *derivedThread)
{
	Thread *thread = (Thread*)derivedThread;
	if(thread == NULL)
		return NULL;

	thread->setStatus(Thread::RUNNING);

	thread->run( );
	
	thread->setStatus(Thread::EXIT);

	//此处需要加锁处理
	g_QuitThreadCount++;

	return NULL;
}

void Thread::run( )
{

}
