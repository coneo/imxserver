#include "stdafx.h"

#include "CLAskLogin.h"
#include "LCRetLogin.h"
#include "imxGSConnect.h"
#include "GatewayPlayer.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

UINT CLAskLoginHandler::Execute( CLAskLogin* pPacket, Player* pPlayer)
{
	printf("#CLAskLoginHandler-------------account:%s,password:%s,version:%u",pPacket->GetAccount(),pPacket->GetPassWord(),pPacket->GetVersion());

	GatewayPlayer* pGatewayPlayer = static_cast<GatewayPlayer*>(pPlayer);
	Assert(pGatewayPlayer);

	/*if(pGatewayPlayer->GetPlayerStatus() != PS_LOGIN_STATUS)
	{
		return PACKET_EXE_ERROR;
	}
	*/

	ServerPlayer*	pServerPlayer = g_pServerManager->GetServerPlayer(SCENE_PLAYER_ID);
	if(pServerPlayer==NULL)
	{
		Assert(false);
		return PACKET_EXE_CONTINUE;
	}

	imxGSConnect msg;
	msg.SetAccount(pPacket->GetAccount());
	msg.SetPlayerID(pPlayer->PlayerID());
	pServerPlayer->SendPacket(&msg);

	/*LCRetLogin msg1;
	msg1.SetAccount(pPacket->GetAccount());
	msg1.SetResult(LOGINR_SUCCESS);
	pGatewayPlayer->SendPacket(&msg1);*/

	return PACKET_EXE_CONTINUE;
}
