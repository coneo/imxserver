
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__


#include "Type.h"
#include "Player.h"


//管理器的上限
#define MAX_PLAYER	1024


class PlayerManager
{
public:
	PlayerManager( );
	~PlayerManager( );

	void 		CleanUp( );

	bool		AddPlayer( Player* pPlayer );

	void		RemovePlayer( PlayerID_t pid );

	PlayerID_t*	GetPlayers( ){ return m_pPlayers; };
	/*玩家数量*/
	UINT	 	GetPlayerNumber( ){ return m_nPlayers; };
	/*是否有玩家存在*/
	bool		HasPlayer( ){ return (bool)m_nPlayers; };

protected:

	PlayerID_t 	m_pPlayers[MAX_PLAYER];		/**/
	UINT	 	m_nPlayers;	/**/

};


#endif
