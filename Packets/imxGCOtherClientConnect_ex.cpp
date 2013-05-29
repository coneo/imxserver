#include "stdafx.h"

#include "imxGCOtherClientConnect_ex.h"

BOOL imxGCOtherClientConnect_ex::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&playerID), sizeof(PlayerID_t));

	return TRUE;
}

BOOL imxGCOtherClientConnect_ex::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&playerID), sizeof(PlayerID_t));
	
	return TRUE;
}

UINT imxGCOtherClientConnect_ex::Execute(Player* pPlayer)
{
	return imxGCOtherClientConnect_exHandler::Execute( this, pPlayer);
}

const CHAR*	imxGCOtherClientConnect_ex::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxGCOtherClientConnect_ex::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}

PlayerID_t imxGCOtherClientConnect_ex::GetPlayerID( ) const
{
	return playerID;
}

VOID		imxGCOtherClientConnect_ex::SetPlayerID(PlayerID_t playerid)
{
	playerID = playerid;
}
