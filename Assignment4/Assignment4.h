
// Assignment4.h : main header file for the Assignment4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAssignment4App:
// See Assignment4.cpp for the implementation of this class
//

class CAssignment4App : public CWinApp
{
public:
	CAssignment4App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAssignment4App theApp;
