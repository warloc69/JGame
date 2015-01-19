
#include <windows.h>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include "Exception.h"
#include "Logger.h"

Exception::Exception(std::wstring msg)
{ 
	m_message = msg;
	m_stack_trace = generateStackTrace();
}

std::wstring Exception::getMessage()
{
	return m_message;
}

std::wstring Exception::getStackTrace()
{
	return m_stack_trace;
}

std::wstring Exception::generateStackTrace()
{
	HANDLE process = GetCurrentProcess();

	SymSetOptions(SYMOPT_LOAD_LINES);
	SymInitializeW(process, NULL, TRUE);

	const short maxFrames = 64;
	void* stack[maxFrames];
	unsigned short frames = CaptureStackBackTrace(0, maxFrames - 1, stack, NULL);
	SYMBOL_INFO* symbol   = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen    = 255;
	symbol->SizeOfStruct  = sizeof(SYMBOL_INFO);

	std::wstring stackStr = L"Handled exception [" + m_message + L"] at:";
	IMAGEHLP_LINE64 line;
	DWORD displ;
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	for(short i = 2; i < frames; i++)
	{
		SymGetLineFromAddr64(process, (DWORD64) stack[i], &displ, &line);
		SymFromAddr(process, (DWORD64) stack[i], NULL, symbol);
		stackStr += L"\n >>> 0x" + toWString(symbol->Address) 
			+ L" " + toWString(line.FileName)
			+ L" :: " + toWString(symbol->Name)
			+ L" (line:" + toWString(line.LineNumber) + L")";
	}
	free(symbol);

	return stackStr;
}

void Exception::printUnhandledExceptionST()
{
	HANDLE process = GetCurrentProcess();

	SymSetOptions(SYMOPT_LOAD_LINES);
	SymInitializeW(process, NULL, TRUE);

	const short maxFrames = 64;
	void* stack[maxFrames];
	unsigned short frames = CaptureStackBackTrace(0, maxFrames - 1, stack, NULL);
	SYMBOL_INFO* symbol   = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen    = 255;
	symbol->SizeOfStruct  = sizeof(SYMBOL_INFO);

	std::wstring stackStr = L"Unhandled exception at:";
	IMAGEHLP_LINE64 line;
	DWORD displ;
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	for(short i = 0; i < frames; i++)
	{
		SymGetLineFromAddr64(process, (DWORD64) stack[i], &displ, &line);
		SymFromAddr(process, (DWORD64) stack[i], NULL, symbol);
		stackStr += L"\n >>> 0x" + toWString(symbol->Address) 
			+ L" " + toWString(line.FileName)
			+ L" :: " + toWString(symbol->Name)
			+ L" (line:" + toWString(line.LineNumber) + L")";
	}
	free(symbol);

	Logger::error(stackStr, true);
}