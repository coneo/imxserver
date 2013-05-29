#include "stdafx.h"

#include "PlayerPool.h"

PlayerPool*  g_pPlayerPool = NULL;

PlayerPool::PlayerPool( )
{
	m_pPlayer	 = NULL;
	m_Position 	 = 0;
	m_PlayerCount 	 = 0;
	m_MaxPlayerCount = 0;
}

PlayerPool::~PlayerPool( )
{
	SAFE_DELETE_ARRAY( m_pPlayer );
}

bool PlayerPool::Init( UINT MaxPlayerCount )
{
	m_pPlayer = new ScenePlayer[MaxPlayerCount];
	if(!m_pPlayer) return false;

	for(UINT i=0; i<MaxPlayerCount; i++)
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i );
		m_pPlayer[i].SetEmpty(true);
	}
	m_Position = 0;
	m_PlayerCount = MaxPlayerCount;
	m_MaxPlayerCount = MaxPlayerCount;

	return true;
}

ScenePlayer* PlayerPool::GetPlayer( PlayerID_t PlayerID )
{
	if(PlayerID==INVALID_ID || PlayerID>=(PlayerID_t)m_MaxPlayerCount || PlayerID<0)
		return NULL;

	return &(m_pPlayer[PlayerID]);
}

ScenePlayer* PlayerPool::NewPlayer( )
{
	int iRet = 0;
	for(UINT i=0; i<m_MaxPlayerCount; i++)
	{
		if(m_pPlayer[m_Position].IsEmpty( ) )
		{
			iRet = m_Position;
			m_pPlayer[m_Position].SetEmpty(false);

			m_Position ++;
			if( m_Position >= m_MaxPlayerCount )
				m_Position = 0;

			m_PlayerCount --;

			return &(m_pPlayer[iRet]);
		}

		m_Position ++;
		if(m_Position >= m_MaxPlayerCount )
			m_Position = 0;
	}

	return NULL;
}

ScenePlayer* PlayerPool::NewPlayer( PlayerID_t PlayerID )
{
	if(PlayerID >= (PlayerID_t)m_MaxPlayerCount )
	{
		return NULL;
	}

	if(m_pPlayer[PlayerID].IsEmpty( ))
	{
		ScenePlayer* pPlayer = &(m_pPlayer[PlayerID]);
		pPlayer->SetEmpty(false);
		m_PlayerCount --;
		m_Position = PlayerID;
		m_Position ++;
		if( m_Position >= m_MaxPlayerCount )
			m_Position = 0;

		return pPlayer;
	}

	return NULL;
}

void PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
	if((UINT)PlayerID >= m_MaxPlayerCount )
	{
		//assert false;
		return;
	}
	m_pPlayer[PlayerID].SetEmpty( true );
	m_PlayerCount ++;

	return ;
}
