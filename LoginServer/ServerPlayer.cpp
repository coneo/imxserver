#include "stdafx.h"

#include "ServerPlayer.h"
#include "PacketFactoryManager.h"

ServerPlayer::ServerPlayer(bool bIsServer)
{
	m_Status = 0;
}

ServerPlayer::~ServerPlayer( )
{
}

bool ServerPlayer::Init( )
{
	SetDisconnect(false);
	return true;
}

bool ServerPlayer::ProcessInput( )
{
	return Player::ProcessInput( );
}

bool ServerPlayer::ProcessOutput( )
{
	return Player::ProcessOutput( );
}

bool ServerPlayer::ProcessCommand(bool option)
{
	return Player::ProcessCommand(option);
}

bool ServerPlayer::SendPacket(Packet* pPacket)
{
	return Player::SendPacket( pPacket );
}

bool ServerPlayer::IsValid( )
{
	if(!Player::IsValid())
		return false;

	return true;
}

void ServerPlayer::CleanUp( )
{
	Player::CleanUp( );
}

bool ServerPlayer::HeartBeat( UINT uTime )
{
	return Player::HeartBeat(uTime);
}

void ServerPlayer::FreeOwn( )
{

}
