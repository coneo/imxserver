#include<stdio.h>
#include<stdarg.h>
#include<string>
#include "Log.h"

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;


Log*	g_pLog = NULL;
/**
 *\brief 构造函数，设置配置文件（有待更灵活的设计）
 */
Log::Log( const std::string& filename)
{
	DOMConfigurator::configure(filename);
	debuglogger	= (Logger::getLogger("debug"));
	infologger	= (Logger::getLogger("info"));
	warnlogger	= (Logger::getLogger("warn"));
	errorlogger = (Logger::getLogger("error"));
}

/**
 *\析构
 */
Log::~Log( )
{
}

/**
 *\brief bug日志
 *\lpszFormat 日志参数，形似printf参数
 */
void Log::log_debug(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	char szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	LOG4CXX_DEBUG(debuglogger, szBuffer);
}

/**
 *\brief info日志
 *\lpszFormat 日志参数，形似printf参数
 */
void Log::log_info(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	char szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	LOG4CXX_INFO(infologger, szBuffer);
}

/**
 *\brief warn日志
 *\lpszFormat 日志参数，形似printf参数
 */
void Log::log_warn(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	char szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	LOG4CXX_WARN(warnlogger, szBuffer);
}

/**
 *\brief error日志
 *\lpszFormat 日志参数，形似printf参数
 */
void Log::log_error(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	char szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	LOG4CXX_ERROR(errorlogger, szBuffer);
}
