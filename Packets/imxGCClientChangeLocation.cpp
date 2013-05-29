#include "stdafx.h"

#include "imxGCClientChangeLocation.h"

BOOL imxGCClientChangeLocation::Read(SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_X), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_Y), sizeof(FLOAT));

	return TRUE;
}

BOOL imxGCClientChangeLocation::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_X), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_Y), sizeof(FLOAT));
	
	return TRUE;
}

UINT imxGCClientChangeLocation::Execute(Player* pPlayer)
{
	return imxGCClientChangeLocationHandler::Execute( this, pPlayer);
}

const CHAR*	imxGCClientChangeLocation::GetAccount( ) const
{
	return szAccount;	
}

VOID		imxGCClientChangeLocation::SetAccount(const CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount, pAccount, MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0';
}

