
#ifndef __GATEWAYPLAYER_H__
#define __GATEWAYPLAYER_H__


#include "Type.h"
#include "Player.h"

class GatewayPlayer : public Player
{
public:
	GatewayPlayer( );
	~GatewayPlayer( );
	
	//数据接收接口
	virtual bool	ProcessInput( );
	//数据发送接口
	virtual bool	ProcessOutput( );
	//消息处理接口
	virtual bool	ProcessCommand(bool option = true);
	//心跳处理
	virtual bool	HeartBeat(UINT uTime=0){ return true; }

public:
	void			Init( );

	virtual bool	SendPacket(Packet* pPacket);

	virtual void	Disconnect( );

	virtual void	CleanUp( );

	bool			FreeOwn( );

public:
	const char* 	GetAccount( ){ return NULL; }
	void		SetAccount(const char* account){ }

	UINT		GetVersion( ){ return 0; }
	void		SetVersion(UINT uVersion){  }

private:
	GUID_t		m_AccountGuid;

	UINT		m_Version;
};


#endif
