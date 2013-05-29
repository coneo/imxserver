#include "stdafx.h"
#include "ScenePlayer.h"
#include "PlayerPool.h"

ScenePlayer::ScenePlayer( )
{
}

ScenePlayer::~ScenePlayer( )
{
}

bool ScenePlayer::ProcessInput( )
{
	return Player::ProcessInput( );
}

bool ScenePlayer::ProcessOutput( )
{
	return Player::ProcessOutput( );
}

bool ScenePlayer::ProcessCommand(bool option)
{
	return Player::ProcessCommand(option);
}

void ScenePlayer::Init( )
{
	SetDisconnect( false );
}

bool ScenePlayer::SendPacket( Packet* pPacket)
{
	return Player::SendPacket(pPacket);
}

void ScenePlayer::CleanUp( )
{
	m_AccountGuid		= 0;
	m_Version			= 0;

	return Player::CleanUp( );	
}

bool ScenePlayer::FreeOwn( )
{
	CleanUp( );
	g_pPlayerPool->DelPlayer( PlayerID() );

	return true;
}

void ScenePlayer::Disconnect( )
{
	SetDisconnect(true);

	Player::Disconnect( );
}
