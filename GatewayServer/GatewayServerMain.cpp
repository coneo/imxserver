#include "stdafx.h"
#include "GatewayServer.h"
#include "Log.h"

int main( )
{
	g_pLog = new Log("Config/gatewaylogConfig.xml");
	if(!g_pLog)
	{
		Assert(false);	
	}


	GatewayServer  gateway;
	gateway.Init( );
	gateway.Loop( );
	gateway.Exit( );

	SAFE_DELETE(g_pLog);

	return 0;
}
