#include "stdafx.h"

#include "imxGSConnect.h"

BOOL imxGSConnect::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&playerID), sizeof(PlayerID_t));

	return TRUE;
}

BOOL imxGSConnect::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&playerID), sizeof(PlayerID_t));
	
	return TRUE;
}

UINT imxGSConnect::Execute(Player* pPlayer)
{
	return imxGSConnectHandler::Execute( this, pPlayer);
}

const CHAR*	imxGSConnect::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxGSConnect::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}

PlayerID_t imxGSConnect::GetPlayerID( ) const
{
	return playerID;
}

VOID	imxGSConnect::SetPlayerID(PlayerID_t playerid)
{
	playerID = playerid;	
}
