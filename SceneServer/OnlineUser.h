#ifndef __ONLINE_USER_H__
#define __ONLINE_USER_H__

#include "Type.h"
#include "User.h"
#include "Packet.h"
#include<map>

//考虑用hashmap来储存User，提高查找效率
class OnlineUser
{
public:
	OnlineUser( );
	~OnlineUser( );

	bool		Init(UINT maxUserCount = MAX_POOL_SIZE);

	User*		GetUser(ID_t	userID);

	User*		NewUser( ){ return NULL; }
	User*		NewUser(ID_t	userID);
	void		DelUser(ID_t	userID);

	bool		SendPacketToAll(Packet* pPacket);

	INT			GetTotalCount( ){ return m_UserCount; }

private:
	//__gnu_cxx::hash_map<ID_t,User*> mUserList;
	typedef std::map<ID_t,User*>::iterator UserMapIterator;
	std::map<ID_t,User*> mUserMap;

	INT	m_UserCount;	//在线玩家数量

};

extern OnlineUser*	g_pOnlineUser;

#endif
