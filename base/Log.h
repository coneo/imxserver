/**
 *\brief 日志模块，按照指定的格式来写日志
 *\使用方法：包含"Log.h"头文件，然后直接log_debug(...)或log_warn(...)或log_info(...)或log_error(...)
 *
 * 日志格式：log_XXX("[模块]user,id,内容");
 * 例如：log_debug("[装备]-hxq-24-获取装备");
 */

#ifndef _LOG_H__
#define _LOG_H__

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <string>

using namespace std;

class Log
{
public:
	Log(const std::string& filename);
	~Log( );

	void log_debug(const char* lpszFormat, ...);
	void log_info(const char* lpszFormat, ...);
	void log_warn(const char* lpszFormat, ...);
	void log_error(const char* lpszFormat, ...);

protected:

	void WriteDebugLog(const char* lpszFormat, ... );
	void WriteInfoLog(const char* lpszFormat, ...);
	void WriteWarnLog(const char* lpszFormat, ...);
	void WriteErrorLog(const char* lpszFormat, ...);
	
	log4cxx::LoggerPtr debuglogger;
	log4cxx::LoggerPtr infologger;
	log4cxx::LoggerPtr warnlogger;
	log4cxx::LoggerPtr errorlogger;

private:

	//只允许显式构造函数
	Log( ){ }
};

extern Log* g_pLog;


#endif
