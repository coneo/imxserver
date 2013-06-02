/**
 *Version:0.0.1
 *Date:2013-3-23
 *Author: 
 */

#ifndef _BASETYPE_H_
#define _BASETYPE_H_


/**
 *\预定义操作系统类型
 */
#ifndef __LINUX__
	#define __LINUX__
#endif



/**
 *\定义头文件包含情况
 */
#include <sys/types.h>
#include <pthread.h>
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <unistd.h>
#include <setjmp.h>
#include <sys/epoll.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdarg.h>


/**
*\基本类型定义
*/
#define VOID				void
typedef unsigned char		UCHAR;
typedef char				CHAR;
typedef unsigned int		UINT;
typedef int					INT;
typedef unsigned short	 	USHORT;
typedef short				SHORT;
typedef unsigned long		ULONG;
typedef long				LONG;
typedef float				FLOAT;

typedef UCHAR				uchar;
typedef USHORT				ushort;
typedef ULONG				ulong;
typedef ULONG				IP_t;
typedef USHORT				PacketID_t;
typedef INT					BOOL;
typedef UCHAR				BYTE;



/*****
*\基本宏定义
*
*/
#define INVALID_ID		-1

#define IP_SIZE			24


#define PACK_COMPART "$-$"   	//封包分隔符
#define PACK_COMPART_SIZE	strlen(PACK_COMPART)

#define OVER_MAX_SERVER 256

/******
*\清楚内存操作
*
*/
//根据指针删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针删除数组内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//根据指针调用Release接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif



//真
#ifndef TRUE
	#define TRUE 1
#endif
//假
#ifndef FALSE
	#define FALSE 0
#endif

/**
 *\调试预定义宏定义
 *
 */
#define __ENTER_FUNCTION_FOXNET if(1){
#define __LEAVE_FUNCTION_FOXNET }

#define _MY_TRY try
#define _MY_CATCH catch(...)




#endif
