#include "stdafx.h"

#include "imxSGRetConnect.h"
#include "LCRetLogin.h"
#include "GatewayPlayer.h"
#include "PlayerPool.h"

UINT imxSGRetConnectHandler::Execute(imxSGRetConnect* pPacket, Player* pPlayer)
{
	std::cout<<"#imxSGRetConnectHandler-------------------"<<std::endl;

	GatewayPlayer* gGatewayPlayer = g_pPlayerPool->GetPlayer(pPacket->GetPlayerID());
	if(!gGatewayPlayer)
	{
		Assert(false);
		std::cout<<"gGatewayPlayer none of packetID:"<<pPacket->GetPlayerID()<<std::endl;
	}
	LCRetLogin msg;
	msg.SetAccount(pPacket->GetAccount());
	msg.SetResult(LOGINR_SUCCESS);
	gGatewayPlayer->SendPacket(&msg);

	return PACKET_EXE_CONTINUE;
}
