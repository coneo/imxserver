#ifndef __SERVER_PLAYER_H__
#define __SERVER_PLAYER_H__

#include "Type.h"
#include "Player.h"

class ServerPlayer : public Player
{
public:
	ServerPlayer(bool bIsServer=true);
	~ServerPlayer( );

	//处理数据流入
	virtual bool	ProcessInput( );
	//处理数据流出
	virtual bool	ProcessOutput( );
	//处理缓冲区数据
	virtual bool	ProcessCommand(bool option=true);

	virtual void	CleanUp( );
	virtual bool	HeartBeat(UINT uTime=0);

public:
	virtual bool	IsServerPlayer(){return true;};
	
	void			SetPlayerStatus( UINT status){m_Status = status;};

	virtual bool	IsValid( );

	virtual bool	SendPacket(Packet* pPacket);

	virtual void	Encrypt_SC(char* header, UINT uLen, UINT uBeginPlace){}

	virtual void	DecryptHead_CS(char* header){;}

	virtual void 	Decrypt_CS(char* header,UINT ulen, UINT uBeginPlace){}

private:
	UINT			m_Status;
};
#endif
