#include "stdafx.h"

#include "imxGCClientChangeLocation.h"

UINT imxGCClientChangeLocationHandler::Execute( imxGCClientChangeLocation* pPacket, Player* pPlayer)
{
	std::cout<<"#imxGCClientChangeLocationHandler--------\n";

	return PACKET_EXE_CONTINUE;
}
