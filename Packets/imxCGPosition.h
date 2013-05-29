#ifndef _IMX_CGPOSITION_H__
#define _IMX_CGPOSITION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class imxCGPosition : public Packet
	{
	public:
		imxCGPosition( )
		{
		}
		virtual ~imxCGPosition( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_CG_POSITION ;}
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

	class imxCGPositionFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxCGPosition( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_CG_POSITION; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(FLOAT)+
					sizeof(FLOAT);
		}
	};

	class imxCGPositionHandler
	{
	public:
		static UINT Execute( imxCGPosition* pPacket, Player* pPlayer);
	};





	/*
	 *imxCGPosition_ex
	 *
	 */
	class imxCGPosition_ex : public Packet
	{
	public:
		imxCGPosition_ex( )
		{
		}
		virtual ~imxCGPosition_ex( ){ }

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t 		GetPacketID( ) const { return PACKET_IMX_CG_POSITION_EX ;}
		virtual UINT			GetPacketSize( ) const
		{
			return 				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(FLOAT)+
								sizeof(FLOAT)+
								sizeof(PlayerID_t);
		}

	public:
		//使用数据接口
		const CHAR*				GetAccount( ) const;
		VOID					SetAccount(const CHAR* pAccount);

		FLOAT					GetPositionX( ) const{ return m_X;}
		VOID					SetPositionX(FLOAT x){ m_X = x;}

		FLOAT					GetPositionY( ) const{ return m_Y;}
		VOID					SetPositionY(FLOAT y){ m_Y = y;}

		PlayerID_t				GetPlayerID( ) const{ return playerID;}
		VOID					SetPlayerID(PlayerID_t playerid)
		{
			playerID = playerid;
		}

	private:
		CHAR					szAccount[MAX_ACCOUNT];
		FLOAT					m_X;
		FLOAT					m_Y;
		PlayerID_t				playerID;
	};

	class imxCGPosition_exFactory : public PacketFactory
	{
		Packet*		CreatePacket( ){ return new imxCGPosition_ex( ); }
		PacketID_t	GetPacketID( )const { return PACKET_IMX_CG_POSITION_EX; }
		UINT		GetPacketMaxSize( ) const
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(FLOAT)+
					sizeof(FLOAT)+
					sizeof(PlayerID_t);
		}
	};

	class imxCGPosition_exHandler
	{
	public:
		static UINT Execute( imxCGPosition_ex* pPacket, Player* pPlayer);
	};
};

using namespace Packets;

#endif
