// ***********************************************************************
// Code by James Michael Armstrong (https://github.com/BlazesRus)
// Multi-view code based on https://www.codeproject.com/Articles/7686/Using-Multiview
// ***********************************************************************
#pragma once
#if !defined(NodeViewApp_IncludeGuard)
#define NodeViewApp_IncludeGuard

#ifndef __AFXWIN_H__
	#error include 'StdBhvTree.h' before including this file for PCH
#endif

#include "AppResources.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// NodeViewApp:
// See MultiView.cpp for the implementation of this class
//

class NodeViewApp : public CWinApp
{
	CView* m_pPrimaryView;
	CView* m_pSecondaryView;
public:
	NodeViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NodeViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(NodeViewApp)
	afx_msg void OnAppAbout();
	afx_msg void OnViewPrimaryView();
	afx_msg void OnViewSecondaryView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NodeViewApp.h__8BA04ED9_F781_43FC_8BF5_D83442C4A9B7__INCLUDED_)
