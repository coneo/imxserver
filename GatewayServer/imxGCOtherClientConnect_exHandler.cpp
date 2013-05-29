#include "stdafx.h"

#include "imxGCOtherClientConnect_ex.h"
#include "imxGCOtherClientConnect.h"
#include "GatewayPlayerManager.h"
#include "PlayerPool.h"
#include "Log.h"

UINT imxGCOtherClientConnect_exHandler::Execute(imxGCOtherClientConnect_ex* pPacket, Player* pPlayer)
{
	std::cout<<"#imxGCOtherClientConnect_ex ---------------------"<<std::endl;
	imxGCOtherClientConnect msg;
	msg.SetAccount((CHAR*)pPacket->GetAccount());

	UINT playerCount = g_pGatewayPlayerManager->GetPlayerNumber( );
	for(UINT i=0; i<playerCount; i++)
	{
		PlayerID_t playerid = (g_pGatewayPlayerManager->GetPlayers())[i] ;
		if(playerid == INVALID_ID)
			continue;

		if(playerid == pPacket->GetPlayerID()) //排除自己
			continue;

		GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(playerid);
		if(!pPlayer)
			continue;

		pPlayer->SendPacket(&msg);
		g_pLog->log_debug("imxGCOhterClientConnect_ex...SendPacket...accoutn:%s,playerID:%u,packID:%u",pPacket->GetAccount(),playerid,msg.GetPacketID());
	}
	

	return PACKET_EXE_CONTINUE;
}
