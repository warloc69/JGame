
#pragma once

#include <string>

class Exception
{
	public:
		Exception(std::wstring msg);
		virtual ~Exception() {}

		std::wstring getMessage();
		std::wstring getStackTrace();

		static void printUnhandledExceptionST();

	private:
		std::wstring m_message;
		std::wstring m_stack_trace;

		std::wstring generateStackTrace();
};