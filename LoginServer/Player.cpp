/**
 *Version:0.0.1
 *Date:2013-6-2
 *Author: 
 */

#include "stdafx.h"
#include "Player.h"
#include "PacketFactoryManager.h"
#include "Log.h"

using namespace Packets;

Player::Player( )
{
	m_PID		= INVALID_ID;
	m_PlayerManagerID = INVALID_ID;

	m_pSocket = new Socket;
	Assert(m_pSocket);

	m_pSocketInputStream = new SocketInputStream(m_pSocket);
	Assert(m_pSocketInputStream);

	m_pSocketOutputStream = new SocketOutputStream(m_pSocket);
	Assert(m_pSocketOutputStream);

	m_IsEmpty	= true;
	m_IsDisconnect	= false;
}

Player::~Player( )
{
	SAFE_DELETE( m_pSocketInputStream );
	SAFE_DELETE( m_pSocketOutputStream );
	SAFE_DELETE( m_pSocket );
}

void Player::Disconnect( )
{
	m_pSocket->close( );
}

bool Player::IsValid( )
{
	if(m_pSocket==NULL) return false;

	if( !m_pSocket->isValid()) return false;

	return true;
}

void Player::CleanUp( )
{
	m_pSocket->close( );
	m_pSocketInputStream->CleanUp( );
	m_pSocketOutputStream->CleanUp( );
	SetPlayerManagerID( INVALID_ID );
	SetDisconnect( false );
}

bool Player::ProcessInput( )
{
	if( IsDisconnect() )
		return true;

	UINT ret = m_pSocketInputStream->Fill( );
	std::cout<<"SocketInputStream ret="<< ret <<std::endl;

	if( (INT)ret <= SOCKET_ERROR)
	{
		//log
		return false;
	}

	return true;	
}

/**
 *\brief 处理发送缓冲区，发送数据包
 *\return 处理结果
 */
bool Player::ProcessOutput( )
{
	if(IsDisconnect( ))
		return true;

	UINT size = m_pSocketOutputStream->Length( );
	if(size==0)
	{
		return true;
	}

	UINT ret = m_pSocketOutputStream->Flush( );
	if((INT)ret <= SOCKET_ERROR)
	{
		std::cout<<"m_pSocketOutputStream Flush error"<<std::endl;
		return false;
	}

	return true;
}

/**
 *处理接入的数据包，解密包，分解包，执行包的操作
 *\return 返回操作是否成功
 */
bool Player::ProcessCommand( bool option )
{
	bool ret = false;

	CHAR header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	UINT packetuint = 0;
	UINT packetSize = 0;
	UINT packetIndex = 0;
	UINT packetTick = 0;
	//Packet* pPacket = NULL;

	if( IsDisconnect())
		return true;

	if(option)
	{
	}

	for( ;; )
	{
		CHAR compart[PACK_COMPART_SIZE]={0};
		if(!m_pSocketInputStream->Find(compart))
			break;

		if(!m_pSocketInputStream->Peek(&header[0], PACKET_HEADER_SIZE))
		{
			break;
		}

		memcpy( &packetID, &header[0], sizeof(PacketID_t) );
		memcpy( &packetTick, &header[sizeof(PacketID_t)], sizeof(UINT) );
		memcpy( &packetuint, &header[sizeof(UINT) + sizeof(PacketID_t)], sizeof(UINT) );
		packetSize = GET_PACKET_LEN(packetuint);
		packetIndex = GET_PACKET_INDEX(packetuint);

		std::cout<<" PacketID:"<<packetID<<" packetTick:"<<packetTick<<" packetuint:"<<packetuint<<" packetSize:"<<packetSize<<" packetIndex:"<<packetIndex<<std::endl;

		if(packetID >= (PacketID_t)PACKET_MAX)
		{
			Assert(false);
			return false;
		}
		//消息解密
		//
		//

		if( m_pSocketInputStream->Length()<PACKET_HEADER_SIZE+packetSize)
		{
			break;
		}

		Packet* pPacket = g_pPacketFactoryManager->CreatePacket(packetID);
		if(pPacket == NULL)
		{
			Assert(false);
			return false;
		}

		std::cout<<"Create packet success\n";

		pPacket->SetPacketIndex(packetIndex);

		ret = m_pSocketInputStream->ReadPacket(pPacket);
		if(ret==false)
		{
			Assert(false);
			g_pPacketFactoryManager->RemovePacket(pPacket);

			return false;
		}

		bool bNeedRemove = true;

		//this->ResetKick( );
		
		//执行包操作
		UINT uret = pPacket->Execute( this );
		if(uret == PACKET_EXE_ERROR)
		{
			if(pPacket)
				g_pPacketFactoryManager->RemovePacket(pPacket);

			return false;
		}
		else if(uret == PACKET_EXE_BREAK)
		{
			if(pPacket)
				g_pPacketFactoryManager->RemovePacket(pPacket);

			break;
		}
		else if(uret==PACKET_EXE_CONTINUE)
		{//继续解析剩下消息
		}
		else if(uret==PACKET_EXE_NOTREMOVE)
		{//继续解析剩下消息，不回收
			bNeedRemove = false;
		}
		else if(uret==PACKET_EXE_NOTREMOVE_ERROR)
		{
			return false;
		}
		else
		{
			Assert(false);
		}


		if(pPacket && bNeedRemove)
			g_pPacketFactoryManager->RemovePacket(pPacket);
	}


	return true;
}

bool Player::SendPacket(Packet* pPacket)
{
	if(IsDisconnect())
		return true;

	if(m_pSocketOutputStream!=NULL)
	{
		pPacket->SetPacketIndex(m_PacketIndex++);

		const PacketID_t packetID = pPacket->GetPacketID( );
		UINT w;
		if(packetID < PACKET_MAX/*PACKET_LC_MAXCOUNT*/)
		{
			w = m_pSocketOutputStream->Write(PACK_COMPART, PACK_COMPART_SIZE);
			//g_pLog->log_debug("%u,%u,Write ... PACK_COMPART:%u",0,0,PACK_COMPART_SIZE);
		}

		//UINT t_uTail_Begin = m_pSocketOutputStream->GetTail( );


		w = m_pSocketOutputStream->Write( (CHAR *)&packetID, sizeof(PacketID_t));
		//g_pLog->log_debug("%u,%u,Write ... packetID size:%u,data:%u",0,0,sizeof(PacketID_t),packetID);

		UINT packetTick = 2;
		w = m_pSocketOutputStream->Write((CHAR *)&packetTick, sizeof(UINT));
		//g_pLog->log_debug("%u,%u,Write ... packetTick size:%u,data:%u",0,0,sizeof(UINT),packetTick);

		UINT packetUINT;
		UINT packetSize = pPacket->GetPacketSize( );
		UINT packetIndex = pPacket->GetPacketIndex( );

		SET_PACKET_INDEX(packetUINT, packetIndex);
		SET_PACKET_LEN(packetUINT, packetSize);

		std::ostringstream sspacketUINT;
		sspacketUINT << packetUINT;
		w = m_pSocketOutputStream->Write( (CHAR *)&packetUINT, sizeof(UINT));
		//g_pLog->log_debug("%u,%u,Write ... packetUINT size:%u,data:%u",0,0,sizeof(UINT),packetUINT);

		BOOL ret = pPacket->Write(*m_pSocketOutputStream);
		//g_pLog->log_debug("%u,%u,Write ... packet:%u",0,0,pPacket->GetPacketSize());

		g_pLog->log_debug("%u,%u,Gateway SendPacket() .port:%u,packID:%u,packSize:%u",PlayerID(),0,m_pSocketOutputStream->m_pSocket->m_Port,pPacket->GetPacketID(),pPacket->GetPacketSize());

		//UINT t_uTail_End = m_pSocketOutputStream->GetTail( );


		//加密
		//
		//

		Assert(ret);

	}
	return true;
}
