
#ifndef _IMX_GSCONNECT_H__
#define _IMX_GSCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxGSConnect : public Packet
	{
	public:
		imxGSConnect( )
		{
			//memset(szAccount,0,MAX_ACCOUNT);
			playerID = 0;
		}
		virtual ~imxGSConnect( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_GS_CONNECT ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(PlayerID_t);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		PlayerID_t		GetPlayerID( ) const;
		VOID					SetPlayerID(PlayerID_t playerid);

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		PlayerID_t				playerID;
	};

	class imxGSConnectFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxGSConnect( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_GS_CONNECT; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(PlayerID_t);
		}
	};

	class imxGSConnectHandler
	{
	public:
		static UINT Execute( imxGSConnect* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
