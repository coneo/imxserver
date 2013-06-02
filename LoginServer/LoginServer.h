#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "Type.h"

class LoginServer
{
public:
	LoginServer( );
	~LoginServer( );

	bool		Init( );
	bool		Loop( );
	bool		Exit( );

protected:
	bool		NewStaticManager( );
	bool		InitStaticManager( );
	bool		DelStaticManager( );

};

extern LoginServer g_LoginServer;

#endif
