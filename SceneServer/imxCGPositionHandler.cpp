#include "stdafx.h"

#include "imxCGPosition.h"
#include "ScenePlayer.h"
#include "imxGCClientChangeLocation.h"

UINT imxCGPositionHandler::Execute(imxCGPosition* pPacket, Player* pPlayer)
{
//	std::cout<<"#imxCGPositionHandler---------------------------m_X:"<<pPacket->GetPositionX()<<" m_Y:"<<pPacket->GetPositionY()<<std::endl;

	ScenePlayer*	pScenePlayer = static_cast<ScenePlayer*>(pPlayer);
	Assert(pScenePlayer);

	imxGCClientChangeLocation msg;
	msg.SetAccount(pPacket->GetAccount());
	msg.SetPositionX(pPacket->GetPositionX());
	msg.SetPositionY(pPacket->GetPositionY());
	pScenePlayer->SendPacket(&msg);

	return PACKET_EXE_CONTINUE;
}


UINT imxCGPosition_exHandler::Execute(imxCGPosition_ex* pPacket, Player* pPlayer)
{	
	return PACKET_EXE_CONTINUE;
}
