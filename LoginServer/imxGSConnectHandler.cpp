#include "stdafx.h"

#include "imxGSConnect.h"
//#include "imxSGRetConnect.h"
//#include "imxGCOtherClientConnect_ex.h"
//#include "ScenePlayer.h"
//#include "User.h"
//#include "OnlineUser.h"
#include "Log.h"

UINT imxGSConnectHandler::Execute(imxGSConnect* pPacket, Player* pPlayer)
{
	/*std::cout<<"#imxGSConnectHandler-------------playerid:"<<pPacket->GetPlayerID()<<"---------\n";

	ScenePlayer*	pScenePlayer = static_cast<ScenePlayer*>(pPlayer);
	Assert(pScenePlayer);

	User* pUser = g_pOnlineUser->NewUser(pPacket->GetPlayerID());
	if(!pUser)
	{
		g_pLog->log_error("%u,%u,g_pOnlineUser->NewUser Failed",0,0);	
		//发送失败信息
		return PACKET_EXE_CONTINUE;
	}

	imxSGRetConnect msg;
	msg.SetAccount(pPacket->GetAccount());
	msg.SetPlayerID(pPacket->GetPlayerID());
	msg.SetResult(CONNECT_SUCCESS);
	pScenePlayer->SendPacket(&msg);

	imxGCOtherClientConnect_ex msg2;
	msg2.SetAccount(pPacket->GetAccount());
	msg2.SetPlayerID(pPacket->GetPlayerID());
	pScenePlayer->SendPacket(&msg2);
	*/

	return PACKET_EXE_CONTINUE;
}
