
#pragma once

#include <string>
#include <sstream>
#include "baseTypes.h"

enum LogLevel
{
	ERROR_LEVEL = 0,
	INFO,
	WARN,
	DEBUG,
	TRACE,
};

#define LOG_LEVEL LogLevel::TRACE

template <class T> std::wstring toWString(const T & t)
{
	std::wostringstream oss;
	oss.clear();
	oss << t;
	return oss.str();
};

template <class T> std::string toString(const T & t)
{
	std::ostringstream oss;
	oss.clear();
	oss << t;
	return oss.str();
};

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