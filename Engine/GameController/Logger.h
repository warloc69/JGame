
#pragma once

#include "baseTypes.h"
#include "Macro.h"

enum LogLevel
{
	ERROR_LEVEL = 0,
	INFO,
	WARN,
	DEBUG,
	TRACE,
};

#define LOG_LEVEL LogLevel::TRACE

class Logger
{
	public:
		static void info(const std::wstring str, bool showWindow = false);
		static void warn(const std::wstring str, bool showWindow = false);
		static void debug(const std::wstring str, bool showWindow = false);
		static void trace(const std::wstring str, bool showWindow = false);
		static void error(const std::wstring str, bool showWindow = false);

		static std::wstring getLocalTime();
		static uint64 getCurrentTime();

	private:
		static void log(LogLevel level, const std::wstring str, bool showWindow);
};