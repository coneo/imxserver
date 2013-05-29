#include "stdafx.h"

#include "OnlineUser.h"
#include "Log.h"

OnlineUser*	g_pOnlineUser = NULL;

OnlineUser::OnlineUser( )
{
}

OnlineUser::~OnlineUser( )
{
}

bool OnlineUser::Init(UINT maxUserCount)
{
	return true;
}

User* OnlineUser::GetUser(ID_t userID)
{
	UserMapIterator iter = mUserMap.find(userID);
	if(iter != mUserMap.end())//find
	{
		return iter->second;
	}
	return NULL;
}

User* OnlineUser::NewUser(ID_t userID )
{
	User* user = new User( userID );
	if(!user)
		return NULL;
	
	mUserMap.insert(std::make_pair(userID,user));

	m_UserCount++;

	g_pLog->log_debug("%u,%u,NewUser ... OK",userID,0);

	g_pLog->log_debug("%u,%u,OnlineUserCount ... %d",userID,0,GetTotalCount());

	return user;
}

void OnlineUser::DelUser(ID_t userID)
{
	UserMapIterator iter = mUserMap.find(userID);
	if(iter != mUserMap.end()) //find
	{	
		User* temp = iter->second;
		mUserMap.erase(iter);
		SAFE_DELETE(temp);
	}
	m_UserCount--;
}

bool OnlineUser::SendPacketToAll(Packet* pPacaket)
{
	return true;	
}
