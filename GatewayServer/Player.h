/**
 *Version:0.0.1
 *Date:2013-3-31
 *Author: 
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Type.h"
#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"


#define MAX_CACHE_SIZE 1024

enum PACKET_FLAG
{
	PF_NONE = 0,
	PF_REMOVE,
};

struct ASYNC_PACKET
{	
	Packet* 	m_pPacket;
	PlayerID_t	m_PlayerID;
	UINT		m_Flag;

	ASYNC_PACKET( )
	{
		m_pPacket = NULL;
		m_PlayerID = INVALID_ID;
		m_Flag = PF_NONE;
	};

	~ASYNC_PACKET()
	{
		SAFE_DELETE(m_pPacket);
		m_PlayerID = INVALID_ID;
		m_Flag = PF_NONE;
	};
};


class Player
{
public:
	Player( );
	~Player( );

	//数据流入接口
	virtual bool	ProcessInput( );
	//数据发出接口
	virtual bool	ProcessOutput( );
	//消息处理接口
	virtual bool	ProcessCommand(bool option = true);
	//心跳检测接口
	virtual bool	HeartBeat(UINT uTime=0 ){ return true; }

	virtual bool 	SendPacket(Packet* pPacket);

public:
	//获取和管理PlayerPool管理的玩家ID
	PlayerID_t	PlayerID( ){ return m_PID; }
	void		SetPlayerID(PlayerID_t id){ m_PID = id; }

	//获取和设置由PlayerManager管理的玩家ID	
	ID_t		PlayerManagerID( ){ return m_PlayerManagerID; }
	void		SetPlayerManagerID( ID_t id){ m_PlayerManagerID = id; }

	//获取当前玩家的套接字入口
	Socket*		GetSocket(){ return m_pSocket; }

	virtual	void	Disconnect( );
	virtual bool	IsValid( );
	virtual void	CleanUp( );

	//判断当前玩家的数据块是否为空,如果为空，
	//则可被PlayerPool分配出去用于新连接
	//\return: 数据块为空
	bool		IsEmpty( ){ return m_IsEmpty; }
	void		SetEmpty(bool opt=true ){ m_IsEmpty = opt; }

	//判断当前玩家是否处于断开状态	
	bool		IsDisconnect( ){ return m_IsDisconnect; }
	void		SetDisconnect(bool opt=true){ m_IsDisconnect = opt; }	
private:
	bool		m_IsEmpty;	/*玩家数据块是否为空*/
	bool		m_IsDisconnect; /*是否处于断开状态*/

protected:
	PlayerID_t	m_PID;		/*标识PlayerPool中的玩家*/
	
	ID_t		m_PlayerManagerID; /*由PlayerManager管理的玩家ID*/

	//网络连接
	Socket*		m_pSocket;

	//数据缓冲区
	SocketInputStream*		m_pSocketInputStream;
	SocketOutputStream*		m_pSocketOutputStream;

	//包的索引
	BYTE					m_PacketIndex;
};

#endif
