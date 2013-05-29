
#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__


#include "Type.h"
#include "ScenePlayer.h"

class PlayerPool
{
public:
	PlayerPool( );
	~PlayerPool( );

	bool		Init( UINT MaxPlayerCount = MAX_POOL_SIZE );

	ScenePlayer*		GetPlayer( PlayerID_t PlayerID );
	
	ScenePlayer*		NewPlayer( );
	ScenePlayer*		NewPlayer(PlayerID_t PlayerID );

	void			DelPlayer( PlayerID_t PlayerID );

private:
	ScenePlayer*		m_pPlayer;	/*Player列表*/
	UINT			m_Position;	/*玩家池的位置*/

	UINT			m_PlayerCount;	/*池中剩余的玩家数量*/
	UINT			m_MaxPlayerCount; /*池子的最大上限*/

	void			SetPosition(UINT pos)
	{
		m_Position = pos;
	}

};

extern PlayerPool*  g_pPlayerPool;


#endif
