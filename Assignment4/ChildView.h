
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "ShaderWnd\ShaderWnd.h"

// CChildView window

class CChildView : public CShaderWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	virtual void RenderGL();
	virtual void InitGL();
	virtual void CleanGL();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
