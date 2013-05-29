#ifndef _IMX_GCCLIENT_CHANGELOCATION_H__ 
#define _IMX_GCCLIENT_CHANGELOCATION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxGCClientChangeLocation : public Packet
	{
	public:
		imxGCClientChangeLocation( )
		{
		}
		virtual ~imxGCClientChangeLocation( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_GC_CLIENTCHANGELOCATION;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(FLOAT)+
								sizeof(FLOAT);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		FLOAT					GetPositionX( ) const{ return m_X;}
		VOID					SetPositionX(FLOAT x){ m_X = x;}

		FLOAT					GetPositionY( ) const{ return m_Y;}
		VOID					SetPositionY(FLOAT y){ m_Y = y;}

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		FLOAT					m_X;
		FLOAT					m_Y;
	};

	class imxGCClientChangeLocationFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxGCClientChangeLocation( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_GC_CLIENTCHANGELOCATION; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(FLOAT)+
					sizeof(FLOAT);
		}
	};

	class imxGCClientChangeLocationHandler
	{
	public:
		static UINT Execute( imxGCClientChangeLocation* pPacket, Player* pPlayer);
	};

};

using namespace Packets;

#endif
