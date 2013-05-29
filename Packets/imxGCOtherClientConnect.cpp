#include "stdafx.h"

#include "imxGCOtherClientConnect.h"

BOOL imxGCOtherClientConnect::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);

	return TRUE;
}

BOOL imxGCOtherClientConnect::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	
	return TRUE;
}

UINT imxGCOtherClientConnect::Execute(Player* pPlayer)
{
	return imxGCOtherClientConnectHandler::Execute( this, pPlayer);
}

const CHAR*	imxGCOtherClientConnect::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxGCOtherClientConnect::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}
