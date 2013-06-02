#include "stdafx.h"
#include "SceneServer.h"
#include "Log.h"

int main( )
{
	g_pLog = new Log("Config/scenelogConfig.xml");
	if(!g_pLog)
	{
		Assert(false);	
	}

	SceneServer sceneserver;
	sceneserver.Init( );
	sceneserver.Loop( );
	sceneserver.Exit( );

	SAFE_DELETE(g_pLog);

	return 0;
}
