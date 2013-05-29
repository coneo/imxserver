
#ifndef _IMX_GCOTHER_CLIENTCONNECT_EX_H__
#define _IMX_GCOTHER_CLIENTCONNECT_EX_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxGCOtherClientConnect_ex : public Packet
	{
	public:
		imxGCOtherClientConnect_ex( ){ }
		virtual ~imxGCOtherClientConnect_ex( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_GC_OTHERCLIENTCONNECT_EX ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(PlayerID_t);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		PlayerID_t				GetPlayerID( ) const;
		VOID					SetPlayerID(PlayerID_t playerid);

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		PlayerID_t				playerID;
	};

	class imxGCOtherClientConnect_exFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxGCOtherClientConnect_ex( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_GC_OTHERCLIENTCONNECT_EX; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(PlayerID_t);
		}
	};

	class imxGCOtherClientConnect_exHandler
	{
	public:
		static UINT Execute( imxGCOtherClientConnect_ex* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
