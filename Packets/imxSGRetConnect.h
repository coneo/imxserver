#ifndef _IMXSG_RETCONNECT_H__
#define _IMXSG_RETCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxSGRetConnect : public Packet
	{
	public:
		imxSGRetConnect( ){ }
		virtual ~imxSGRetConnect( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_SG_RETCONNECT ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(PlayerID_t) +
								sizeof(CONNECT_RESULT);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		PlayerID_t				GetPlayerID( ) const;
		VOID					SetPlayerID(PlayerID_t playerid);

		CONNECT_RESULT			GetResult( ) const;
		VOID					SetResult(CONNECT_RESULT result);

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		PlayerID_t				playerID;
		CONNECT_RESULT			Result;
	};

	class imxSGRetConnectFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxSGRetConnect( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_SG_RETCONNECT; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(PlayerID_t)+
					sizeof(CONNECT_RESULT);
		}
	};

	class imxSGRetConnectHandler
	{
	public:
		static UINT Execute( imxSGRetConnect* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
