// MultiView.cpp : Defines the class behaviors for the application.
//

#include "StdBhvTree.h"
#include "NodeViewApp.h"

#include "..\DynMultiView\MainFrm.h"
#include "..\DynMultiView\MultiViewDoc.h"
#include "..\DynMultiView\AboutDlg.h"

#include "TreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NodeViewApp

BEGIN_MESSAGE_MAP(NodeViewApp, CWinApp)
	//{{AFX_MSG_MAP(NodeViewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_PRIMARYVIEW, OnViewPrimaryView)
	ON_COMMAND(ID_VIEW_SECONDARYVIEW, OnViewSecondaryView)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NodeViewApp construction

NodeViewApp::NodeViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only NodeViewApp object

NodeViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// NodeViewApp initialization

BOOL NodeViewApp::InitInstance()
{
	AfxEnableControlContainer();
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(MultiViewDoc),
		RUNTIME_CLASS(MainFrame),       // main SDI frame window
		RUNTIME_CLASS(TreeView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CView* pActiveView = ((CFrameWnd*) m_pMainWnd)->GetActiveView();
	m_pPrimaryView = pActiveView;
	m_pSecondaryView = (CView*) new TreeView;

	CDocument* pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();

	CCreateContext context;
	context.m_pCurrentDoc = pDoc;

	UINT m_ID = AFX_IDW_PANE_FIRST + 1;
	CRect rect; 

	m_pSecondaryView->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_ID, &context);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// App command to run the dialog
void NodeViewApp::OnAppAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// NodeViewApp message handlers


void NodeViewApp::OnViewSecondaryView()
{
	// TODO: Add your command handler code here
	UINT temp = ::GetWindowLong(m_pSecondaryView->m_hWnd, GWL_ID);
    ::SetWindowLong(m_pSecondaryView->m_hWnd, GWL_ID, ::GetWindowLong(m_pPrimaryView->m_hWnd, GWL_ID));
    ::SetWindowLong(m_pPrimaryView->m_hWnd, GWL_ID, temp);

	m_pPrimaryView->ShowWindow(SW_HIDE);
	m_pSecondaryView->ShowWindow(SW_SHOW);  	

	((CFrameWnd*)m_pMainWnd)->SetActiveView(m_pSecondaryView);  
	((CFrameWnd*) m_pMainWnd)->RecalcLayout();
    m_pSecondaryView->Invalidate();
	
}

void NodeViewApp::OnViewPrimaryView()
{
 	// TODO: Add your command handler code here
    
    UINT temp = ::GetWindowWord(m_pSecondaryView->m_hWnd, GWL_ID);
    ::SetWindowWord(m_pSecondaryView->m_hWnd, GWL_ID, ::GetWindowWord(m_pPrimaryView->m_hWnd, GWL_ID));
    ::SetWindowWord(m_pPrimaryView->m_hWnd, GWL_ID, temp);
 
 	m_pSecondaryView->ShowWindow(SW_HIDE);
 	m_pPrimaryView->ShowWindow(SW_SHOW);  	
 	
 	((CFrameWnd*)m_pMainWnd)->SetActiveView(m_pPrimaryView);  
 	((CFrameWnd*)m_pMainWnd)->RecalcLayout();
    m_pPrimaryView->Invalidate();
}




