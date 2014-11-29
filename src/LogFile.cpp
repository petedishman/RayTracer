// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "LogFile.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
	// default state for logging options
	FlushFile	= false;
	LogTime		= true;
	AutoEndLine = true;

}

CLogFile::~CLogFile()
{
	if (File.is_open())
		File.close();
}

bool CLogFile::Initialise(TCHAR *Filename, int mode)
{
	// Get a copy of the filename, never know
	//_tcscpy(FileName, Filename);

	_tcscpy_s(FileName, Filename);

	if (mode == APPEND)
	{
		File.open(FileName, std::ios::out | std::ios::app);
	}
	else // mode == OVERWRITE
	{
		File.open(FileName, std::ios::out | std::ios::trunc);
	}

	if (File.is_open())
		return true;
	else
		return false;
}

void CLogFile::EnableTimeLogging(bool enable)
{
	LogTime = enable;
}

void CLogFile::EnableFileFlush(bool enable)
{
	FlushFile = enable;
}

void CLogFile::EnableAutoLineEnd(bool enable)
{
	AutoEndLine = enable;
}

void CLogFile::Log(const TCHAR *text)
{
	if (LogTime)
	{
		SYSTEMTIME timedata;

		GetLocalTime(&timedata);

		File.fill('0');
		std::streamsize oldwidth = File.width(2);

		File << timedata.wHour << ":";
		File.width(2);
		File << timedata.wMinute << ":";
		File.width(2);
		File << timedata.wSecond << ".";
		
		File.width(3);
		File << timedata.wMilliseconds << "   ";
		
		File.width(oldwidth);
	}

	File << text;

	if (AutoEndLine)
		File << std::endl;

	if (FlushFile)
		File.flush();
}

void CLogFile::NewLine()
{
	bool oldtimeinfo;

	oldtimeinfo = LogTime;

	LogTime = false;

	if (AutoEndLine)
		Log(_T(""));
	else
		Log(_T("\n"));

	LogTime = oldtimeinfo;
}