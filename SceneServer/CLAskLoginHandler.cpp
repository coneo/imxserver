#include "stdafx.h"

#include "CLAskLogin.h"

UINT CLAskLoginHandler::Execute( CLAskLogin* pPacket, Player* pPlayer)
{
	std::cout<<"#CLAskLoginHandler--------\n";

	return PACKET_EXE_CONTINUE;
}
