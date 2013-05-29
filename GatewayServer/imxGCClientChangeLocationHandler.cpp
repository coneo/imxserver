#include "stdafx.h"

#include "imxGCClientChangeLocation.h"
#include "GatewayPlayerManager.h"
#include "PlayerPool.h"
#include "Log.h"

UINT imxGCClientChangeLocationHandler::Execute( imxGCClientChangeLocation* pPacket, Player* pPlayer)
{
	UINT playerCount = g_pGatewayPlayerManager->GetPlayerNumber( );
	for(UINT i=0; i<playerCount; i++)
	{
		PlayerID_t playerid = (g_pGatewayPlayerManager->GetPlayers())[i] ;
		if(playerid == INVALID_ID)
			continue;

	//	if(playerid == pPacket->GetPlayerID()) //排除自己
	//		continue;

		GatewayPlayer* pPlayer = g_pPlayerPool->GetPlayer(playerid);
		if(!pPlayer)
			continue;

		pPlayer->SendPacket(pPacket);
		g_pLog->log_debug("imxGCClientChangeLocation...SendPacket...accoutn:%s,playerID:%u,packID:%u",pPacket->GetAccount(),playerid,pPacket->GetPacketID());
	}

	return PACKET_EXE_CONTINUE;
}
