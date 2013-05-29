#ifndef _LCRET_LOGIN_H__
#define _LCRET_LOGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LCRetLogin : public Packet
	{
	public:
		LCRetLogin( ){ }
		virtual ~LCRetLogin( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_LC_RETLOGIN ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(LOGIN_RESULT);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		LOGIN_RESULT			GetResult( ) const;
		VOID					SetResult(LOGIN_RESULT result);

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		LOGIN_RESULT			Result;
	};

	class LCRetLoginFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new LCRetLogin( ); }
		PacketID_t	GetPacketID( )const { return PACKET_LC_RETLOGIN; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(LOGIN_RESULT);
		}
	};

	class LCRetLoginHandler
	{
	public:
		static UINT Execute( LCRetLogin* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
