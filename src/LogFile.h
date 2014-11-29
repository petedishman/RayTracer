// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////
//
// 01/04/2000  Peter Dishman
//
// Class for debug logging to files and general file output.
// One instance of a class per logfile

#if !defined(AFX_LOGFILE_H__AB203A00_080D_11D4_A281_B02355C10000__INCLUDED_)
#define AFX_LOGFILE_H__AB203A00_080D_11D4_A281_B02355C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>

#define APPEND       1 
#define OVERWRITE    2

class CLogFile  
{
public:
	void EnableAutoLineEnd(bool enable);
	void Log(const TCHAR* text);
	void NewLine();
	void EnableFileFlush(bool enable);
	void EnableTimeLogging(bool enable);
	bool Initialise(TCHAR *Filename, int mode);
	CLogFile();
	virtual ~CLogFile();

	TCHAR FileName[30];

	std::ofstream File;
	bool FlushFile;
	bool LogTime;
	bool AutoEndLine;
};

#endif // !defined(AFX_LOGFILE_H__AB203A00_080D_11D4_A281_B02355C10000__INCLUDED_)
