#include "stdafx.h"

#include "CLAskLogin.h"


BOOL CLAskLogin::Read( SocketInputStream& iStream ) 
{
		iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
		iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
		iStream.Read( (CHAR*)(&uVersion), sizeof(UINT));
	
		/*UINT m_PacketSize = sizeof(CHAR)*MAX_ACCOUNT + sizeof(CHAR)*MAX_PASSWORD + sizeof(UINT);
		CHAR* Msg = new CHAR[m_PacketSize];
		iStream.Read(Msg, m_PacketSize);
		m_CLAskLogin.ParseFromString(std::string(Msg, m_PacketSize));
		delete[] Msg;*/
			
	return TRUE ;
}

BOOL CLAskLogin::Write( SocketOutputStream& oStream )const
{
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
	oStream.Write( (CHAR*)(&uVersion), sizeof(UINT));

	/*std::string str;
	m_CLAskLogin.SerializeToString(&str);
	oStream.Write((CHAR*)str.c_str(), str.size() );*/

	return TRUE ;
}

UINT CLAskLogin::Execute( Player* pPlayer )
{
	return CLAskLoginHandler::Execute( this, pPlayer ) ;
}

const	CHAR*	CLAskLogin::GetAccount()	const
{
	return szAccount;
	//return m_CLAskLogin.account().c_str();
}
VOID	CLAskLogin::SetAccount(CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = 0 ;
	//m_CLAskLogin.set_account(pAccount);
}

const	CHAR*	CLAskLogin::GetPassWord()	const
{
	return szPassWord;
	//return m_CLAskLogin.password().c_str();
}
VOID			CLAskLogin::SetPassWord(CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_PASSWORD);
	szPassWord[MAX_PASSWORD] = 0 ;
	//m_CLAskLogin.set_password(pPassWord);
}

UINT			CLAskLogin::GetVersion() const
{
	return	uVersion;
	//return m_CLAskLogin.clientversion();
}

VOID			CLAskLogin::SetVersion(UINT version)
{
	uVersion = version;
	//m_CLAskLogin.set_clientversion(version);
}

