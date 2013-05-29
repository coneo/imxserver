/**
 *Version:0.0.1
 *Date:2013-3-23
 *Author: 
 */


#ifndef _GATEWAYSERVER_H_
#define _GATEWAYSERVER_H_

#include "Type.h"

class GatewayServer
{
public:
	GatewayServer( );
	~GatewayServer( );

public:
	bool		Init( );
	bool		Loop( );
	bool		Exit( );

	void		Stop( );

protected:
	bool		NewGatewayServer( );
	bool		InitGatewayServer( );
	bool 		m_bExited;
};



#endif
