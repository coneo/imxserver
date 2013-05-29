#include "stdafx.h"

#include "imxSGRetConnect.h"

BOOL imxSGRetConnect::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&playerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(&Result), sizeof(CONNECT_RESULT));

	return TRUE;
}

BOOL imxSGRetConnect::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&playerID), sizeof(PlayerID_t));
	oStream.Write( (CHAR*)(&Result), sizeof(CONNECT_RESULT));
	
	return TRUE;
}

UINT imxSGRetConnect::Execute(Player* pPlayer)
{
	return imxSGRetConnectHandler::Execute( this, pPlayer);
}

const CHAR*	imxSGRetConnect::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxSGRetConnect::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}

PlayerID_t imxSGRetConnect::GetPlayerID( ) const
{
	return playerID;
}

VOID	imxSGRetConnect::SetPlayerID(PlayerID_t playerid)
{
	playerID = playerid;
}

CONNECT_RESULT imxSGRetConnect::GetResult( ) const
{
	return Result;
}

VOID	imxSGRetConnect::SetResult(CONNECT_RESULT result)
{
	Result = result;
}
