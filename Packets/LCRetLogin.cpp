#include "stdafx.h"

#include "LCRetLogin.h"

BOOL LCRetLogin::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&Result), sizeof(LOGIN_RESULT));

	return TRUE;
}

BOOL LCRetLogin::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&Result), sizeof(LOGIN_RESULT));
	
	return TRUE;
}

UINT LCRetLogin::Execute(Player* pPlayer)
{
	return LCRetLoginHandler::Execute( this, pPlayer);
}

const CHAR*	LCRetLogin::GetAccount( ) const
{
	return szAccount;	
}

VOID		LCRetLogin::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}

LOGIN_RESULT LCRetLogin::GetResult( ) const
{
	return Result;
}

VOID	LCRetLogin::SetResult(LOGIN_RESULT result)
{
	Result = result;
}
