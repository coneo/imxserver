#include "stdafx.h"
#include "GatewayPlayer.h"
#include "PlayerPool.h"

GatewayPlayer::GatewayPlayer( )
{
}

GatewayPlayer::~GatewayPlayer( )
{
}

bool GatewayPlayer::ProcessInput( )
{
	return Player::ProcessInput( );
}

bool GatewayPlayer::ProcessOutput( )
{
	return Player::ProcessOutput( );
}

bool GatewayPlayer::ProcessCommand(bool option)
{
	return Player::ProcessCommand(option);
}

void GatewayPlayer::Init( )
{
	SetDisconnect( false );
}

bool GatewayPlayer::SendPacket( Packet* pPacket)
{
	return Player::SendPacket(pPacket);
}

void GatewayPlayer::CleanUp( )
{
	m_AccountGuid		= 0;
	m_Version			= 0;

	return Player::CleanUp( );	
}

bool GatewayPlayer::FreeOwn( )
{
	CleanUp( );
	g_pPlayerPool->DelPlayer( PlayerID() );

	return true;
}

void GatewayPlayer::Disconnect( )
{
	SetDisconnect(true);

	Player::Disconnect( );
}
