
// UNOgame.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUNOgameApp:
// See UNOgame.cpp for the implementation of this class
//

class CUNOgameApp : public CWinApp
{
public:
	CUNOgameApp();
	ULONG_PTR m_gdiplusToken;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUNOgameApp theApp;
