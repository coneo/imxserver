#include "stdafx.h"

#include "PlayerManager.h"
#include "PlayerPool.h"

PlayerManager::PlayerManager( )
{
	m_nPlayers = 0;

	for( int i=0; i<MAX_PLAYER; i++)
	{
		m_pPlayers[i] = INVALID_ID;		
	}
}

PlayerManager::~PlayerManager( )
{
	CleanUp( );
}


void PlayerManager::CleanUp( )
{
	m_nPlayers = 0;

	for( int i=0; i<MAX_PLAYER; i++)
	{
		m_pPlayers[i] = INVALID_ID;		
	}
}

bool PlayerManager::AddPlayer( Player *pPlayer )
{
	if(!pPlayer) return false;

	Assert(pPlayer);
	if(m_pPlayers[m_nPlayers] == INVALID_ID )
	{
		m_pPlayers[m_nPlayers] = pPlayer->PlayerID( );
		pPlayer->SetPlayerManagerID(m_nPlayers);
	
		m_nPlayers ++;
		Assert(m_nPlayers <= MAX_PLAYER );
		if(m_nPlayers > MAX_PLAYER)
			return false;
	}
	else
	{
		Assert(false);
	}

	return true;
}

void PlayerManager::RemovePlayer( PlayerID_t pid )
{
	Assert( m_nPlayers > 0);

	Player* pPlayer = g_pPlayerPool->GetPlayer(pid);
	if(pPlayer==NULL)
	{
		Assert(false);
		return;
	}

	ID_t PlayerManagerID = pPlayer->PlayerManagerID( );
	if(PlayerManagerID < 0 || PlayerManagerID >= MAX_PLAYER)
	{
		std::cout<<PlayerManagerID<<std::endl;
		Assert(false);
		return;
	}

	pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[m_nPlayers-1]);
	if(pPlayer==NULL)
	{
		Assert(false);
		return;
	}

	m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1];
	m_pPlayers[m_nPlayers-1] = INVALID_ID;

	pPlayer->SetPlayerManagerID( PlayerManagerID );

	m_nPlayers --;
	Assert(m_nPlayers>=0);

}
