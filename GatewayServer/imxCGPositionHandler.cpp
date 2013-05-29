#include "stdafx.h"

#include "imxCGPosition.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "CLAskLogin.h"

UINT imxCGPositionHandler::Execute(imxCGPosition* pPacket, Player* pPlayer)
{
	//std::cout<<"#imxCGPositionHandler---------------------------m_X:"<<pPacket->GetPositionX()<<" m_Y:"<<pPacket->GetPositionY()<<std::endl;

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer(SCENE_PLAYER_ID);
	if(!pServerPlayer)
	{
		Assert(false);
		return PACKET_EXE_CONTINUE;
	}

	/*imxCGPosition_ex msg;
	msg.SetAccount(pPacket->GetAccount());
	msg.SetPositionX(32);
	msg.SetPositionY(22);
	msg.SetPlayerID(pPlayer->PlayerID());*/

	pServerPlayer->SendPacket(pPacket);


	return PACKET_EXE_CONTINUE;
}

UINT imxCGPosition_exHandler::Execute(imxCGPosition_ex* pPacket, Player* pPlayer)
{	
	return PACKET_EXE_CONTINUE;
}
