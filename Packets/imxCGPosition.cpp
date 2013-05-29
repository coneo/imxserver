#include "stdafx.h"

#include "imxCGPosition.h"

BOOL imxCGPosition::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_X), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_Y), sizeof(FLOAT));

	return TRUE;
}

BOOL imxCGPosition::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_X), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_Y), sizeof(FLOAT));
	
	return TRUE;
}

UINT imxCGPosition::Execute(Player* pPlayer)
{
	return imxCGPositionHandler::Execute( this, pPlayer);
}

const CHAR*	imxCGPosition::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxCGPosition::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}




/*
 *
 *
 */
BOOL imxCGPosition_ex::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_X), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_Y), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&playerID),sizeof(PlayerID_t));

	return TRUE;
}

BOOL imxCGPosition_ex::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_X), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_Y), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&playerID), sizeof(PlayerID_t));
	
	return TRUE;
}

UINT imxCGPosition_ex::Execute(Player* pPlayer)
{
	return imxCGPosition_exHandler::Execute( this, pPlayer);
}

const CHAR*	imxCGPosition_ex::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxCGPosition_ex::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}
