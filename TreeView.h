// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Tree Code based on https://www.codeproject.com/Articles/9887/CStaticTreeCtrl-A-CStatic-derived-custom-Tree-cont
// ***********************************************************************
#pragma once
#ifndef TreeView_Header
#define TreeView_Header

#include "StdBhvTree.h"
#include <string>
#include <typeinfo>
#include "..\BasicXMLGUI\UIntVector.h"
#include "DataNode.h"
#include "RootNode.h"
#include "InfoDataDictionary.h"
#include "DataDictionary.h"
#include "UIntDic.h"
#include "APPDefines.h"

#include "ContextMenu.h"
#include "TreeNodeText.h"
//#include "TemplateMacros.h"
//#include "IniData/IndexedDictionary.h"
//#include "VariableLists/VariableTypeLists.h"
#include "..\DynMultiView\MultiViewDoc.h"
#include "QuadVector\QuadVector.h"

//#include "ArgList.h"
//#include "VariableTypeLists.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// <summary>
/// View for displaying information from .BHV format files converted into readable format
/// </summary>
/// <seealso cref="CView" />
class TreeView : public CView
{
    DECLARE_DYNCREATE(TreeView)
public:// Construction

    /// <summary>
    /// Initializes a new instance of the <see cref="TreeView"/> class.
    /// </summary>
    TreeView()
    {
        m_iIndent = 16;				// Indentation for tree branches
        m_iPadding = 4;				// Padding between tree and the control border

        m_bShowLines = TRUE;				// Show lines by default
        m_bScrollBarMessage = FALSE;			// Only relevant when calculating the scrollbar

        m_iDocHeight = 0;				// A polite yet meaningless default

        m_crDefaultTextColor = RGB(58, 58, 58);
        m_crConnectingLines = RGB(128, 128, 128);

        // Safeguards
        SetTextFont(8, FALSE, FALSE, "Arial Unicode MS");
    }

    /// <summary>
    /// Finalizes an instance of the <see cref="TreeView"/> class.
    /// </summary>
    virtual ~TreeView()
    {
        Reset();
        m_Font.DeleteObject();
    }

    MultiViewDoc* GetDocument();

    // Attributes
protected:
    LOGFONT			m_lgFont;
    CFont			m_Font;
    COLORREF		m_crDefaultTextColor;
    COLORREF		m_crConnectingLines;
    const COLORREF	m_ArgColor = RGB(202, 201, 201);//(Lighter Shade of Sonic Silver) https://www.schemecolor.com/sample?getcolor=7a7978

    BOOL			m_bShowLines;

    int				m_iDocHeight;
    BOOL			m_bScrollBarMessage;

    int				m_iLineHeight;
    int				m_iIndent;
    int				m_iPadding;

    UIntVector RootNodes;
    DataDictionary NodeBank;
    UIntDic NodeLinks;

    /// <summary>
    /// The node bank holding all event data(names->eventInfos)
    /// </summary>
    InfoDataDictionary EventBank;
    /// <summary>
    /// The node bank holding all variable data(names->
    /// </summary>
    InfoDataDictionary VariableBank;

    /// <summary>
    /// Node bank 
    /// </summary>
    InfoDataDictionary AttriNameBank;
    /// <summary>
    /// Nodes bank holding Character Property info(names->role+flags+type)
    /// </summary>
    InfoDataDictionary CharPropBank;

    //Places all root-nodes at or below X coordinate of RootEnd
    const long RootEnd = 41;
    RootNode TreeStart = "Tree Data";
    RootNode EventDataStart = "Event Data";
    RootNode VariableDataStart = "Variable Data";
    RootNode AttriNameStart = "Attribute Names";
    RootNode CharPropStart = "Character Properties";

/*
    NodeBank.Add("Linked Conditional Tree", 1);//Condition Classes linked together to other nodes in more descriptive way
    NodeBank.Add("Usage Linked Tree", 1);//Classes Linked together based on calls to other nodes
*/
    //Index of Node that ClassNodes are within
    unsigned int ClassNodeStart = 0;
    short NodeSearchRange = 0;

    //Holds data of Name from hkbBehaviorGraphStringData(Event & Variable Names)
    std::string StringDataName = "";
    //Holds data of Signature from hkbBehaviorGraphStringData(Event & Variable Names)
    std::string StringDataSignature = "";
    //Holds data of Name from hkbVariableValueSet (Variable Values)
    std::string ValueDataName = "";
    //Holds data of Signature from hkbVariableValueSet (Variable Values)
    std::string ValueDataSignature = "";
    //Holds data of Name from hkbBehaviorGraphData (Variable Types)
    std::string TypeDataName = "";
    //Holds data of Signature from hkbBehaviorGraphData (Variable Types)
    std::string TypeDataSignature = "";

    //Stores link to Hk class of type hkbVariableValueSet
    std::string variableInitialValues;
    //Stores link to Hk class of type hkbBehaviorGraphStringData
    std::string stringData;

    //Typically 4 Element Array of what seems to be coordinate positions
    QuadVectorList quadVariableValues;
//-------File Loading Operations-------------------------------------------------------

    /// <summary>
    /// Loads the data from file.
    /// </summary>
    /// <param name="FilePath">The file path.</param>
    /// <returns>bool</returns>
    bool LoadDataFromFile(std::string FilePath);

    /// <summary>
    /// Saves the loaded data to file.
    /// </summary>
    /// <param name="FilePath">The file path.</param>
    void SaveDataToFile(std::string FilePath);

//--------------------------------------------------------------------------------------

    /// <summary>
    /// Resets the storage of this instance.
    /// </summary>
    void Reset()
    {
        RootNodes.clear();
        NodeBank.clear();
        NodeLinks.clear();

        EventBank.clear();
        VariableBank.clear();
        AttriNameBank.clear();
        CharPropBank.clear();
    }

    // Operations
public:
    /// <summary>
    /// Sets the text font.
    /// </summary>
    /// <param name="nHeight">Height of the n.</param>
    /// <param name="bBold">The b bold.</param>
    /// <param name="bItalic">The b italic.</param>
    /// <param name="csFaceName">Name of the cs face.</param>
    virtual void SetTextFont(LONG nHeight, BOOL bBold, BOOL bItalic, const CString& csFaceName)
    {
        m_lgFont.lfHeight = -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
        m_lgFont.lfWidth = 0;
        m_lgFont.lfEscapement = 0;
        m_lgFont.lfOrientation = 0;
        m_lgFont.lfWeight = (bBold) ? FW_BOLD : FW_DONTCARE;
        m_lgFont.lfItalic = (BYTE)((bItalic) ? TRUE : FALSE);
        m_lgFont.lfUnderline = FALSE;
        m_lgFont.lfStrikeOut = FALSE;
        m_lgFont.lfCharSet = DEFAULT_CHARSET;
        m_lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
        m_lgFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        m_lgFont.lfQuality = DEFAULT_QUALITY;
        m_lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

        STRCPY(m_lgFont.lfFaceName, csFaceName);

        if (m_Font.GetSafeHandle() != NULL)
            m_Font.DeleteObject();

        m_Font.CreateFontIndirect(&m_lgFont);

        // Calculate node height for this font
        CDC* pDC = GetDC();
        int		iSaved = pDC->SaveDC();
        CFont* pOldFont = pDC->SelectObject(&m_Font);

        // Calculate the height of this font with a character likely to be 'big'
        // and don't forget to add a little padding
        m_iLineHeight = pDC->GetTextExtent("X").cy + 4;

        pDC->SelectObject(pOldFont);
        pDC->RestoreDC(iSaved);
        ReleaseDC(pDC);
    }
    /// <summary>
    /// Sets the default color of the text.
    /// </summary>
    /// <param name="crText">The cr text.</param>
    virtual void SetDefaultTextColor(COLORREF crText)
    {
        m_crDefaultTextColor = crText;
    }

    /// <summary>
    /// Sets the text settings.
    /// </summary>
    /// <param name="nHeight">Height of the n.</param>
    /// <param name="bBold">The b bold.</param>
    /// <param name="bItalic">The b italic.</param>
    /// <param name="csFaceName">Name of the cs face.</param>
    /// <param name="crText">The cr text.</param>
    virtual void SetTextSettings(LONG nHeight, BOOL bBold, BOOL bItalic, const CString& csFaceName, COLORREF crText)
    {
        m_lgFont.lfHeight = -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
        m_lgFont.lfWidth = 0;
        m_lgFont.lfEscapement = 0;
        m_lgFont.lfOrientation = 0;
        m_lgFont.lfWeight = (bBold) ? FW_BOLD : FW_DONTCARE;
        m_lgFont.lfItalic = (BYTE)((bItalic) ? TRUE : FALSE);
        m_lgFont.lfUnderline = FALSE;
        m_lgFont.lfStrikeOut = FALSE;
        m_lgFont.lfCharSet = DEFAULT_CHARSET;
        m_lgFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
        m_lgFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        m_lgFont.lfQuality = DEFAULT_QUALITY;
        m_lgFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

        STRCPY(m_lgFont.lfFaceName, csFaceName);

        if (m_Font.GetSafeHandle() != NULL)
            m_Font.DeleteObject();

        m_Font.CreateFontIndirect(&m_lgFont);

        // Calculate node height for this font
        CDC* pDC = GetDC();
        int		iSaved = pDC->SaveDC();
        CFont* pOldFont = pDC->SelectObject(&m_Font);

        // Calculate the height of this font with a character likely to be 'big'
        // and don't forget to add a little padding
        m_iLineHeight = pDC->GetTextExtent("X").cy + 4;

        pDC->SelectObject(pOldFont);
        pDC->RestoreDC(iSaved);
        ReleaseDC(pDC);
        m_crDefaultTextColor = crText;
    }

    /// <summary>
    /// Toggle node between open and closed
    /// </summary>
    bool ToggleNode(CPoint point, BOOL bInvalidate = FALSE)
    {
        //1=RootNode;2=InfoNode;3=DataNode
        bool FailedToFindNode = false;
        RootNode* TargetRootNode = nullptr;
        InfoNode* TargetInfoNode = nullptr;
        DataNode* TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            TargetRootNode = RetrieveNearestRootNode(point);
            if (TargetRootNode != nullptr) { ToggleNode(TargetRootNode, bInvalidate); FailedToFindNode = false; }
        }
        else
        {
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                TargetNode = RetrieveNodeByPoint(point);
                if (TargetNode != nullptr) { ToggleNode(TargetNode, bInvalidate); FailedToFindNode = false; }
            }
            else
            {
                TargetInfoNode = RetrieveInfoNodeByPoint(point);
                if (TargetInfoNode != nullptr) { ToggleNode(TargetInfoNode, bInvalidate); FailedToFindNode = false; }
            }
        }
        return FailedToFindNode;
    }

    /// <summary>
    /// Toggle node between open and closed
    /// </summary>
    template <typename NodeType>
    void ToggleNode(NodeType* pNode, BOOL bInvalidate = FALSE)
    {
        ASSERT(pNode != NULL);

        pNode->bOpen = !(pNode->bOpen);

        if (bInvalidate)
            Invalidate();
    }

protected:
    void AddAllSubNodes(DataNode* pNode, UIntVector& TargetNodes)
    {
        DataNode* targetNode;
        for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
        {
            targetNode = this->NodeBank[targetNodeIndex];
            AddAllSubNodes(targetNode, TargetNodes);
        }
    }

    void DeleteNode(unsigned int nodeIndex, BOOL bInvalidate = FALSE)
    {
        UIntVector TargetNodes;
        TargetNodes.push_back(nodeIndex);
        DataNode* pNode = &NodeBank[nodeIndex];
        DataNode* targetNode;
        if (pNode->ParentIndex == 0)
        {
            RootNodes.DeleteFirstMatch(nodeIndex);
        }
        for (UIntVector::iterator targetNodeIndex = (pNode->ChildNodes).begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
        {
            targetNode = &this->NodeBank[*targetNodeIndex];
            AddAllSubNodes(targetNode, TargetNodes);
        }
        for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
        {
            NodeBank.Remove(*targetNodeIndex);
        }
        if (bInvalidate)
            Invalidate();
    }

    int DrawNodesFromRoot(CDC* pDC, int x, int y, CRect rFrame)
    {
        int		iDocHeight = 0;		// Total document height
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        EventDataStart.CoordData.CopyRect(rNode);		// Record the rectangle

    // MULTILINE TEXT - begins
        CString	cs = EventDataStart.NodeName.c_str();
        InfoNode* targetInfoNode = nullptr;
        int		iPos;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);

        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        iDocHeight += EventDataStart.CoordData.Height();
        if(EventDataStart.bOpen)
        {
            for (UIntVector::iterator targetNodeIndex = EventBank.RootNodes.begin(), EndIndex = EventBank.RootNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetInfoNode = &this->EventBank[*targetNodeIndex];
                iDocHeight = DrawRecursiveInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 0);
            }
        }

        cs = VariableDataStart.NodeName.c_str();
        rNode.top += rNode.Height();
        rNode.bottom += rNode.Height() + m_iLineHeight;
        VariableDataStart.CoordData.CopyRect(rNode);
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        iDocHeight += VariableDataStart.CoordData.Height();
        if(VariableDataStart.bOpen)
        {
            for (UIntVector::iterator targetNodeIndex = VariableBank.RootNodes.begin(), EndIndex = VariableBank.RootNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetInfoNode = &this->VariableBank[*targetNodeIndex];
                iDocHeight = DrawRecursiveInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 1);
            }
        }

        cs = AttriNameStart.NodeName.c_str();
        rNode.top += rNode.Height();
        rNode.bottom += rNode.Height() + m_iLineHeight;
        AttriNameStart.CoordData.CopyRect(rNode);
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        iDocHeight += AttriNameStart.CoordData.Height();
        if(AttriNameStart.bOpen)
        {
            for (UIntVector::iterator targetNodeIndex = AttriNameBank.RootNodes.begin(), EndIndex = AttriNameBank.RootNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetInfoNode = &this->AttriNameBank[*targetNodeIndex];
                iDocHeight = DrawRecursiveInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 2);
            }
        }

        cs = CharPropStart.NodeName.c_str();
        rNode.top += rNode.Height();
        rNode.bottom += rNode.Height() + m_iLineHeight;
        CharPropStart.CoordData.CopyRect(rNode);
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        iDocHeight += CharPropStart.CoordData.Height();
        if(CharPropStart.bOpen)
        {
            for (UIntVector::iterator targetNodeIndex = CharPropBank.RootNodes.begin(), EndIndex = CharPropBank.RootNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetInfoNode = &this->CharPropBank[*targetNodeIndex];
                iDocHeight = DrawRecursiveInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 3);
            }
        }
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
        rNode.top += rNode.Height();
        rNode.bottom += rNode.Height() + m_iLineHeight;
        TreeStart.CoordData.CopyRect(rNode);
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        iDocHeight += TreeStart.CoordData.Height();
        if(TreeStart.bOpen)
        {
            DataNode* targetNode;
            for (UIntVector::iterator CurrentVal = RootNodes.begin(), LastVal = RootNodes.end(); CurrentVal != LastVal; ++CurrentVal)
            {
                targetNode = &NodeBank[*CurrentVal];
                iDocHeight = DrawRecursiveNodes(pDC, targetNode, RootEnd, y + rNode.Height(), rFrame);
            }
        }

        return iDocHeight;
    }

    int DrawRecursiveInfoNodes(CDC* pDC, InfoNode* pNode, int x, int y, CRect rFrame, short BankType);

    int DrawRecursiveNodes(CDC* pDC, DataNode* pNode, int x, int y, CRect rFrame);

    int HowMuchTextFits(CDC* pDC, int iAvailableWidth, CString csText)
    {
        int iValidSoFar = csText.GetLength() - 1;					// Assume the entire text fits

        // If the text's pixel width is larger than what's available
        if (pDC->GetTextExtent(csText).cx > iAvailableWidth)
        {
            int iNextBlank = 0;	// Position of the next blank in text
            int iPixelWidth = 0;	// Text's pixel width

            // Loop until we can fit no more of the text
            while (iPixelWidth < iAvailableWidth)
            {
                iValidSoFar = iNextBlank;							// Record the char pos so far
                iNextBlank = csText.Find(' ', iNextBlank + 1);	// Advance one word at a time

                // Have reached the end of the string?
                if (iNextBlank == -1)
                    iNextBlank = csText.GetLength();

                // Calculate the new width
                iPixelWidth = pDC->GetTextExtent(csText.Left(iNextBlank)).cx;
            }
        }

        return iValidSoFar;
    }

    void DrawLinesFromRoot(CDC* pDC)
    {
    }

    void DrawLinesRecursive(CDC* pDC, DataNode* pNode);

    void ResetScrollBar()
    {
        // Flag to avoid a call from OnSize while resetting the scrollbar
        m_bScrollBarMessage = TRUE;

        CRect rFrame;

        GetClientRect(rFrame);

        // Need for scrollbars?
        if (rFrame.Height() > m_iDocHeight + 8)
        {
            ShowScrollBar(SB_VERT, FALSE);	// Hide it
            SetScrollPos(SB_VERT, 0);
        }
        else
        {
            SCROLLINFO	si;
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_PAGE | SIF_RANGE;
            si.nPage = rFrame.Height();
            si.nMax = m_iDocHeight + 8;
            si.nMin = 0;

            SetScrollInfo(SB_VERT, &si);
            EnableScrollBarCtrl(SB_VERT, TRUE);
        }

        m_bScrollBarMessage = FALSE;
    }

    BOOL NodeTextDlg(CString& csText)
    {
        BOOL				bRet = FALSE;
        TreeNodeText	tntDlg;

        tntDlg.m_csItemText = csText;

        if (tntDlg.DoModal() == IDOK)
        {
            csText = tntDlg.m_csItemText;
            bRet = TRUE;
        }

        return bRet;
    }

    void AddNode(std::string TagName, unsigned int parentIndex = 0, ArgList args)
    {
    }

    RootNode* RetrieveNearestRootNode(CPoint point)
    {
        RootNode* targetNode = nullptr;
        return targetNode;
    }

    InfoNode* RetrieveInfoNodeByPoint(CPoint point)
    {
        InfoNode* targetNode = nullptr;
        switch (NodeSearchRange)
        {
        case 1://EventNode NodeTree
            break;
        case 2://VariableNode NodeTree
            break;
        default:
            break;
        }
        return targetNode;
    }

    DataNode* RetrieveNodeByPoint(CPoint point)
    {
        DataNode* targetNode = nullptr;
        return targetNode;
    }

    void AddTagContentNode(unsigned int TargetPIndex)
    {

    }
    // Message handlers

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TreeView)
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_NCHITTEST || message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK)
            return ::DefWindowProc(m_hWnd, message, wParam, lParam);

        return CView::WindowProc(message, wParam, lParam);
    }
    //}}AFX_VIRTUAL

    // Generated message map functions
protected:
    //{{AFX_MSG(TreeView)
protected:
    virtual void OnDraw(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy)
    {
        // Setting the scroll sends its own size message. Prevent it thus avoiding an ugly loop.
        // Other than that, resizing the control means that the tree height may change (word-wrap).
        if (!m_bScrollBarMessage)
            ResetScrollBar();

        CView::OnSize(nType, cx, cy);
    }
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
    {
        int iScrollBarPos = GetScrollPos(SB_VERT);

        CRect rFrame;

        GetClientRect(rFrame);

        switch (nSBCode)
        {
        case SB_LINEUP:
            iScrollBarPos = max(iScrollBarPos - m_iLineHeight, 0);
            break;

        case SB_LINEDOWN:
            iScrollBarPos = min(iScrollBarPos + m_iLineHeight, GetScrollLimit(SB_VERT));
            break;

        case SB_PAGEUP:
            iScrollBarPos = max(iScrollBarPos - rFrame.Height(), 0);
            break;

        case SB_PAGEDOWN:
            iScrollBarPos = min(iScrollBarPos + rFrame.Height(), GetScrollLimit(SB_VERT));
            break;

        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
        {
            SCROLLINFO si;

            ZeroMemory(&si, sizeof(SCROLLINFO));

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_TRACKPOS;

            if (GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS))
                iScrollBarPos = si.nTrackPos;
            else
                iScrollBarPos = (UINT)nPos;
            break;
        }
        }

        SetScrollPos(SB_VERT, iScrollBarPos);

        Invalidate();

        CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
    }

    afx_msg void OnLButtonUp(UINT nFlags, CPoint point)
    {
        //1=RootNode;2=InfoNode;3=DataNode
        short NodeTypeFound = 0;
        RootNode* TargetRootNode = nullptr;
        InfoNode* TargetInfoNode = nullptr;
        DataNode* TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            TargetRootNode = RetrieveNearestRootNode(point);
            if (TargetRootNode != nullptr) { NodeTypeFound = 1; }
        }
        else
        {
            if (point > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                TargetNode = RetrieveNodeByPoint(point);
                if (TargetNode != nullptr) { NodeTypeFound = 3; }
            }
            else
            {
                TargetInfoNode = RetrieveNodeByPoint(point);
                if (TargetInfoNode != nullptr) { NodeTypeFound = 2; }
            }
        }
        if (ToggleNode(point, bInvalidate)) { CView::OnLButtonUp(nFlags, point); }
    }

    /// <summary>
    /// Called when [mouse wheel].
    /// </summary>
    /// <param name="nFlags">The n flags.</param>
    /// <param name="zDelta">The z delta.</param>
    /// <param name="pt">The pt.</param>
    /// <returns>BOOL.</returns>
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
    {
        // zDelta greater than 0, means rotating away from the user, that is, scrolling up
        OnVScroll((zDelta > 0) ? SB_LINEUP : SB_LINEDOWN, 0, NULL);

        return CView::OnMouseWheel(nFlags, zDelta, pt);
    }

    /// <summary>
    /// Called when [context menu].
    /// </summary>
    /// <param name="pWnd">The p WND.</param>
    /// <param name="point">The point.</param>
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point)
    {
        CPoint cp(point);

        // WM_CONTEXTMENU passes absolute coordinates, we need them local
        ScreenToClient(&cp);

        //1=RootNode;2=InfoNode;3=DataNode
        short NodeTypeFound = 0;
        RootNode* targetRootNode = nullptr;
        InfoNode* TargetInfoNode = nullptr;
        DataNode* TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            targetRootNode = RetrieveNearestRootNode(point);
            if (targetRootNode != nullptr) { NodeTypeFound = 1; }
        }
        else
        {
            if (point > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                TargetNode = RetrieveNodeByPoint(point);
                if (TargetNode != nullptr) { NodeTypeFound = 3; }
            }
            else
            {
                TargetInfoNode = RetrieveInfoNodeByPoint(point);
                if (TargetInfoNode != nullptr) { NodeTypeFound = 2; }
            }
        }

        if (NodeTypeFound != 0)
        {
            ContextMenu contextMenuPopUp(&m_Font);

            contextMenuPopUp.CreatePopupMenu();

            // Get a device context so that it'll be possible for the context menu
            // to calculate the size of the menu item's text
            CDC* pDC = GetDC();
            int		iSaved = pDC->SaveDC();
            CFont* pOldFont = pDC->SelectObject(&m_Font);

            // ADDING MENU ITEMS - Start

            CString cs;
            switch (NodeTypeFound)
            {
            case 1:
                cs = targetRootNode->NodeName.c_str();
                cs = cs.Left(45) + ((TargetNode->TagName.size() > 45) ? _T("...") : _T(""));
                break;
            case 2:
                cs = TargetInfoNode->TagName.c_str();
                cs = cs.Left(45) + ((TargetNode->TagName.size() > 45) ? _T("...") : _T(""));
                break;
            default:
                cs = TargetNode->TagName.c_str();
                cs = cs.Left(45) + ((TargetNode->TagName.size() > 45) ? _T("...") : _T(""));
                break;
            }
            contextMenuPopUp.AppendMenuItem(MF_DISABLED, WM_APP, cs, pDC);
            contextMenuPopUp.AppendMenuItem(MF_SEPARATOR, 0, _T(""), pDC);
            if (NodeTypeFound == 1)
            {
                contextMenuPopUp.AppendMenuItem(MF_DISABLED, 0, _T("Setting NodeSearchRange to this section"), pDC);
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Load Tree from file"), pDC);
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Save Tree to .xml file"), pDC);
            }
            else if (NodeTypeFound == 2)
            {
                switch (NodeSearchRange)
                {
                case 1:
                    contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Add New Variable"), pDC);
                    //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Copy Event to Other View"), pDC);
                case 2:
                    contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Add New Variable"), pDC);
                    //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Copy Event to Other View"), pDC);
                default:
                    break;
                }
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Delete Node"), pDC);
            }
            else
            {
                switch (TargetNode->NodeType)
                {
                    //Event Link
                    case 92:
                    case 101:
                    case 111:
                        break;
                    //Variable Link
                    case 93:
                    case 102:
                    case 112:
                        break;
                    //ClassNode Link
                    case 94:
                    case 103:
                    case 113:
                        break;
                    //Character Property Link
                    case 95:
                    case 104:
                    case 114:
                        break;
                    //Attribute Name Link
                    case 96:
                    case 105:
                    case 115:
                        break;
                    default:
                        break;
                }
                if (TargetNode->NodeType == 90)
                {
                    //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Delete Arg Data"), pDC);
                }
                else { contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Delete Node"), pDC); }
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_MODIFYNODETEXT, _T("Modify Node Text"), pDC);
                //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_TOGGLECONNECTINGLINES, _T("Toggle Connecting Lines"), pDC);
            }
            switch (TargetNode->NodeType)
            {
            case 89:
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Load Tree from file"), pDC);
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Save Tree to .xml file"), pDC);
                break;
            case 92:
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Add New Event"), pDC);
                //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Copy Event to Other View"), pDC);
                break;
            case 12:
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Add New Variable"), pDC);
                //contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Copy Event to Other View"), pDC);
                contextMenuPopUp.AppendMenuItem(MF_ENABLED, CM_DELETENODE, _T("Delete Node"), pDC);
                break;
            case 13:
                break;
            default:

                break;
            }
            // ADDING MENU ITEMS - End

            // Display the context menu
            contextMenuPopUp.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

            // Clean up
            pDC->SelectObject(pOldFont);
            pDC->RestoreDC(iSaved);
            ReleaseDC(pDC);
        }
    }

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(TreeView, CView)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(CM_DELETENODE, OnCM_DeleteNode)
    ON_COMMAND(CM_MODIFYNODETEXT, OnCM_ModifyNodeText)
    //ON_COMMAND(CM_TOGGLECONNECTINGLINES, OnCM_ToggleConnectingLines)
END_MESSAGE_MAP()
#endif
