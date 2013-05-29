/**
 *Version 0.0.1
 *Date 2013-4-15
 *Author
 */

#ifndef _SCENESERVER_H__
#define _SCENESERVER_H__

#include "Type.h"

class SceneServer
{
public:
	SceneServer( );
	~SceneServer( );

public:
	bool		Init( );
	bool		Loop( );
	bool		Exit( );
	void		Stop( );

protected:
	bool		NewSceneServer( );
	bool		InitSceneServer( );
	bool		m_bExited;
};

#endif
