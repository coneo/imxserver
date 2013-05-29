/**
 *Version:0.0.1
 *Date:2013-3-28
 *Author: 
 */

#ifndef __TYPE_H__
#define __TYPE_H__

#include "BaseType.h"

////
//\数据类型定义
////
typedef short	ID_t;


typedef ID_t		PlayerID_t;	/*连接玩家ID*/
typedef UINT		GUID_t;		/*32位唯一编号*/
typedef ULONG		TID;

/*
 *
 *\基本头文件引用
 *
 */
#include "Assertx.h"
#include "GameDefine.h"
#include "GameUtil.h"
#include "PacketDefine.h"
#include "ResultDefine.h"

#endif
