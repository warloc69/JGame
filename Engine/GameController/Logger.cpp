
#include <windows.h>
#include <time.h>

#include "Logger.h"

#pragma warning( disable : 4482 ) // disable deprecated warning

uint64 Logger::getCurrentTime()
{
	SYSTEMTIME aTm;
	GetLocalTime(&aTm);
	uint64 t = aTm.wMinute * 60000 + aTm.wSecond * 1000 + aTm.wMilliseconds;
	return t;
}

std::wstring Logger::getLocalTime()
{
	SYSTEMTIME aTm;
	GetLocalTime(&aTm);

	char buf[24];
	_snprintf_s(buf,24,"%02d.%02d.%04d %02d:%02d:%02d.%03d",aTm.wDay,aTm.wMonth,aTm.wYear,aTm.wHour,aTm.wMinute,aTm.wSecond,aTm.wMilliseconds);

	WCHAR time[24];
	MultiByteToWideChar( CP_ACP, 0, buf, -1, time, 256 );

	return toWString(time);
}

void Logger::log(LogLevel level, const std::wstring str, bool showWindow)
{
	std::wstring levelStr = L"[ INFO]";
	switch(level)
	{
		case WARN: levelStr = L"[ WARN]"; break;
		case DEBUG: levelStr = L"[DEBUG]"; break;
		case TRACE: levelStr = L"[TRACE]"; break;
		case ERROR_LEVEL: levelStr = L"[ERROR]"; break;
	}

	std::wstring line = levelStr + L" " + getLocalTime() + L" " + str + L"\n";

	if(LOG_LEVEL >= level)
		OutputDebugStringW(line.c_str());

	if(showWindow)
	{
		if(level == ERROR)
		{
			MessageBoxW(NULL, str.c_str(), L"Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		else if(level == INFO)
		{
			MessageBoxW(NULL, str.c_str(), L"Information", MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
		}
		else if(level == WARN)
		{
			MessageBoxW(NULL, str.c_str(), L"Warning!", MB_OK | MB_ICONWARNING | MB_DEFAULT_DESKTOP_ONLY);
		}
		else
		{
			MessageBoxW(NULL, str.c_str(), levelStr.c_str(), MB_OK | MB_USERICON | MB_DEFAULT_DESKTOP_ONLY);
		}
	}
}

void Logger::debug(const std::wstring str, bool showWindow)
{
	log(LogLevel::DEBUG, str, showWindow);
}

void Logger::warn(const std::wstring str, bool showWindow)
{
	log(LogLevel::WARN, str, showWindow);
}

void Logger::info(const std::wstring str, bool showWindow)
{
	log(LogLevel::INFO, str, showWindow);
}

void Logger::trace(const std::wstring str, bool showWindow)
{
	log(LogLevel::TRACE, str, showWindow);
}

void Logger::error(const std::wstring str, bool showWindow)
{
	log(LogLevel::ERROR_LEVEL, str, showWindow);
}