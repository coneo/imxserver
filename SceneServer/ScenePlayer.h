#ifndef __SCENEPLAYER_H__
#define __SCENEPLAYER_H__


#include "Type.h"
#include "Player.h"

class ScenePlayer : public Player
{
public:
	ScenePlayer( );
	~ScenePlayer( );

	//数据接收接口
	virtual bool	ProcessInput( );
	//数据发送接口
	virtual bool	ProcessOutput( );
	//消息处理接口
	virtual bool	ProcessCommand(bool option=true);
	//心跳检测
	virtual bool	HeartBeat(UINT uTime=0){ return true; }

public:
	void			Init( );

	virtual bool	SendPacket(Packet* pPacket);

	virtual void	Disconnect( );

	virtual void	CleanUp( );

	bool			FreeOwn( );

private:
	GUID_t		m_AccountGuid;

	UINT		m_Version;
};

#endif
