
#ifndef _IMX_GCOTHER_CLIENTCONNECT_H__
#define _IMX_GCOTHER_CLIENTCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxGCOtherClientConnect : public Packet
	{
	public:
		imxGCOtherClientConnect( ){ }
		virtual ~imxGCOtherClientConnect( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_GC_OTHERCLIENTCONNECT ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT;
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

	private:
		CHAR					szAccount[MAX_ACCOUNT];
	};

	class imxGCOtherClientConnectFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxGCOtherClientConnect( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_GC_OTHERCLIENTCONNECT; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT;
		}
	};

	class imxGCOtherClientConnectHandler
	{
	public:
		static UINT Execute( imxGCOtherClientConnect* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
