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

    MultiViewDoc* GetDocument() {return (MultiViewDoc*)m_pDocument; }

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
    void SaveDataToFile(std::string FilePath)
    {

    }

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
    /// <param name="crText">The text color to set to.</param>
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
            targetNode = &this->NodeBank[*targetNodeIndex];
            AddAllSubNodes(targetNode, TargetNodes);
        }
    }

    void AddAllSubNodes(InfoNode* pNode, UIntVector& TargetNodes, short BankType)
    {
        InfoNode* targetNode;
        switch (BankType)
        {
        default:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->EventBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 0);
            }
            break;
        case 1:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->VariableBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 1);
            }
            break;
        case 2:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->AttriNameBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 2);
            }
            break;
        case 3:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->CharPropBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 3);
            }
            break;
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

    void DeleteInfoNode(unsigned int nodeIndex, short BankType, BOOL bInvalidate = FALSE)
    {
        UIntVector TargetNodes;
        TargetNodes.push_back(nodeIndex);
        InfoNode* pNode = nullptr;
        switch(BankType)
        {
        default:
            pNode = &EventBank[nodeIndex];break;
        case 1:
            pNode = &VariableBank[nodeIndex];break;
        case 2:
            pNode = &AttriNameBank[nodeIndex]; break;
        case 3:
            pNode = &CharPropBank[nodeIndex]; break;
        }
        InfoNode* targetNode;
        switch (BankType)
        {
        default:
            if (pNode->ParentIndex == 0)
            {
                EventBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->EventBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 0);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                EventBank.Remove(*targetNodeIndex);
            }
            break;
        case 1:
            if (pNode->ParentIndex == 0)
            {
                VariableBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->VariableBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 0);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                VariableBank.Remove(*targetNodeIndex);
            }
            break;
        case 2:
            if (pNode->ParentIndex == 0)
            {
                AttriNameBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->AttriNameBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 0);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                AttriNameBank.Remove(*targetNodeIndex);
            }
            break;
        case 3:
            if (pNode->ParentIndex == 0)
            {
                CharPropBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->CharPropBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 0);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                CharPropBank.Remove(*targetNodeIndex);
            }
            break;
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

    //BankTypes 0=Event; 1=Variable
    int DrawRecursiveInfoNodes(CDC* pDC, InfoNode* pNode, int x, int y, CRect rFrame, short BankType)
    {
        int		iDocHeight = 0;		// Total document height
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        //pNode->CoordData.CopyRect(rNode);		// Record the rectangle

        COLORREF crOldText = pDC->SetTextColor(m_crDefaultTextColor);

        // MULTILINE TEXT - begins
        CString	cs = pNode->TagName.c_str();
        int		iPos;
        int ArgSize;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

        pDC->SetTextColor(crOldText);
        if (pNode->ChildNodes.empty())
        {
            return pNode->CoordData.Height();
        }
        else if (pNode->bOpen)// If the node is open AND it has children, then draw those
        {
            InfoNode* targetNode = nullptr;
            switch (BankType)
            {
            default:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->EventBank[*targetNodeIndex];
                    iDocHeight = DrawRecursiveInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, 0);
                }
                break;
            case 1:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->VariableBank[*targetNodeIndex];
                    iDocHeight = DrawRecursiveInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, 1);
                }
                break;
            case 2:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->AttriNameBank[*targetNodeIndex];
                    iDocHeight = DrawRecursiveInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, 2);
                }
                break;
            case 3:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->CharPropBank[*targetNodeIndex];
                    iDocHeight = DrawRecursiveInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, 3);
                }
                break;
            }

        }

        return iDocHeight + pNode->CoordData.Height();
    }

    int DrawRecursiveNodes(CDC* pDC, DataNode* pNode, int x, int y, CRect rFrame)
    {
        int		iDocHeight = 0;		// Total document height
        CRect	rNode;
        CRect	TotalNodeArea;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        //pNode->CoordData.CopyRect(rNode);		// Record the rectangle

        COLORREF crOldText = pDC->SetTextColor(m_crDefaultTextColor);

        // MULTILINE TEXT - begins
        CString	cs = pNode->TagName.c_str();
        int		iPos;
        int ArgSize;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        // Draw only if the node is visible
        if (rNode.bottom > 0 && rNode.top < rFrame.bottom)
        {
            //Construct TagButton
            pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);//Temporary Code(Switch to button derivative later)
            rNode.left += iPos;
        }
        //-------------------------------------------------------------------------------------------------------

        size_t NumberArgs, Index, LastElem;
        for (auto const& [key, val] : pNode->ArgData)//Draw rest of Nodes based on Args
        {
            NumberArgs = val.size();
            if (NumberArgs == 0)//Non-Value Element
            {
                cs = " " + key;
                iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
                pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
                rNode.left += iPos;
            }
            else if (NumberArgs>1)//Multiple Linked Arg Buttons
            {
                cs = " " + key + "=\"";
                iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
                pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
                rNode.left += iPos;
                for (std::vector<std::string>::iterator Arg = val.begin(), FirstArg = Arg, LastArg = val.end(); Arg != LastArg; ++Arg)
                {
                    if (Arg != FirstArg)
                    {
                        cs = ", ";
                        pDC->DrawText(cs.Left(3), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
                        rNode.left += 2;
                    }
                    cs = *Arg->c_str();
                    iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
                    pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);//Temporary Code
                    rNode.left += iPos;
                }
                cs = "\"";
                pDC->DrawText(cs.Left(2), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
                rNode.left += 1;
            }
            else//Single Arg Link
            {
                cs = " " + key[0];
                iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
                pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);//Temporary Code
                rNode.left += iPos;
            }
        }
        //if (!pNode.TagContent.empty())
        //{
        //    cs = ">";
        //    pDC->DrawText(cs.Left(2), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        //    rNode.left += 1;
        //    for (vector<std::string>::iterator Arg = TagContent.begin(), FirstArg = Arg, LastArg = TagContent.end(); Arg != LastArg; ++Arg)
        //    {//Create TagContent NodeButton(s)
        //        if (Arg != FirstArg)
        //        {
        //            cs = ", ";
        //            pDC->DrawText(cs.Left(3), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        //            rNode.left += 2;
        //        }
        //        cs = *Arg;
        //        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        //        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);//Temporary Code
        //        rNode.left += iPos;
        //    }
        //}
        pDC->SetTextColor(crOldText);
        if (pNode->ChildNodes.empty())
        {
            return pNode->CoordData.Height();
        }
        else if (pNode->bOpen)// If the node is open AND it has children, then draw those
        {
            DataNode* targetNode = nullptr;
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->NodeBank[*targetNodeIndex];
                iDocHeight = DrawRecursiveNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame);
            }
        }

        return iDocHeight + pNode->CoordData.Height();
    }

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

    /// <summary>
    /// Draws the lines from root.
    /// </summary>
    /// <param name="pDC">The document pointer</param>
    void DrawLinesFromRoot(CDC* pDC)
    {
    }

    /// <summary>
    /// Recursively draw the lines 
    /// </summary>
    /// <param name="pDC">The document pointer</param>
    /// <param name="pNode">The pointer node.</param>
    void RecursivelyDrawLines(CDC* pDC, InfoNode* pNode, short BankType)
    {
        InfoNode* targetNode = nullptr;
        if (pNode->bOpen)
        {
            switch (BankType)//Using Switch to avoid checking type each loop(code same except for bank storage)
            {
            default:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->EventBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 0);
                }
                break;
            case 1:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->VariableBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 1);
                }
                break;
            case 2:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->AttriNameBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 2);
                }
                break;
            case 3:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->CharPropBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 3);
                }
                break;
            }
        }
        // Where is the elbow joint of this connecting line?
        int iJointX = pNode->CoordData.left - m_iIndent - 6;
        int iJointY = pNode->CoordData.top + (m_iLineHeight / 2);
        unsigned int TargetIndex = pNode->ParentIndex;

        // If the parent is not the root, throw a connecting line to it
        if (TargetIndex != 0)
        {
            switch (BankType)
            {
            default:
                targetNode = &this->EventBank[TargetIndex]; break;
            case 1:
                targetNode = &this->VariableBank[TargetIndex]; break;
            case 2:
                targetNode = &this->AttriNameBank[TargetIndex]; break;
            case 3:
                targetNode = &this->CharPropBank[TargetIndex]; break;
            }

            // How far up from the joint is the parent?
            int iDispY = iJointY - targetNode->CoordData.top - (m_iLineHeight / 2);

            // Use 1 pixel wide rectangles to draw lines
            pDC->FillSolidRect(iJointX, iJointY, m_iIndent, 1, m_crConnectingLines);	// Horizontal line
            pDC->FillSolidRect(iJointX, iJointY, 1, -iDispY, m_crConnectingLines);		// Vertical line
        }

        // Put a solid dot to mark a node
        pDC->FillSolidRect(iJointX + m_iIndent - 2, iJointY - 2, 5, 5, m_crConnectingLines);

        // Hollow out the dot if the node has no children
        if (pNode->ChildNodes.size() == 0)
            pDC->FillSolidRect(iJointX + m_iIndent - 1, iJointY - 1, 3, 3, RGB(255, 255, 255));
    }

    /// <summary>
    /// Recursively draw the lines 
    /// </summary>
    /// <param name="pDC">The document pointer</param>
    /// <param name="pNode">The pointer node.</param>
    void RecursivelyDrawLines(CDC* pDC, DataNode* pNode)
    {
        DataNode* targetNode = nullptr;
        if (pNode->bOpen)
        {
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->NodeBank[*targetNodeIndex];
                RecursivelyDrawLines(pDC, targetNode);
            }
        }

        // Where is the elbow joint of this connecting line?
        int iJointX = pNode->CoordData.left - m_iIndent - 6;
        int iJointY = pNode->CoordData.top + (m_iLineHeight / 2);
        unsigned int TargetIndex = pNode->ParentIndex;

        // If the parent is not the root, throw a connecting line to it
        if (TargetIndex != 0)
        {
            targetNode = &this->NodeBank[TargetIndex];
            // How far up from the joint is the parent?
            int iDispY = iJointY - targetNode->CoordData.top - (m_iLineHeight / 2);

            // Use 1 pixel wide rectangles to draw lines
            pDC->FillSolidRect(iJointX, iJointY, m_iIndent, 1, m_crConnectingLines);	// Horizontal line
            pDC->FillSolidRect(iJointX, iJointY, 1, -iDispY, m_crConnectingLines);		// Vertical line
        }

        // Put a solid dot to mark a node
        pDC->FillSolidRect(iJointX + m_iIndent - 2, iJointY - 2, 5, 5, m_crConnectingLines);

        // Hollow out the dot if the node has no children
        if (pNode->ChildNodes.size() == 0)
            pDC->FillSolidRect(iJointX + m_iIndent - 1, iJointY - 1, 3, 3, RGB(255, 255, 255));

    }

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

    int AddNode(std::string TagName, unsigned int parentIndex=0)
    {
        DataNode newNode = TagName;
        int nodeIndex = NodeBank.Add(newNode);
        if (parentIndex != 0) { NodeBank[nodeIndex].ParentIndex = parentIndex; }
        return nodeIndex;
    }

    //Returns index of added node on Tree
    int AddNode(std::string TagName, unsigned int parentIndex, ArgList args)
    {
        DataNode newNode = TagName;
        int nodeIndex = NodeBank.Add(newNode);
        if (parentIndex != 0) { NodeBank[nodeIndex].ParentIndex = parentIndex; }
        NodeBank[nodeIndex].ArgData = args;
        return nodeIndex;
    }

    /// <summary>
    /// Retrieves the nearest root node.
    /// </summary>
    /// <param name="point">The point.</param>
    /// <returns>RootNode *.</returns>
    RootNode* RetrieveNearestRootNode(CPoint point)
    {
        RootNode* targetNode = nullptr;
        if (EventDataStart.CoordData.PtInRect(point)){ return &EventDataStart; }
        else if (VariableDataStart.CoordData.PtInRect(point)) { return &VariableDataStart; }
        else if (AttriNameStart.CoordData.PtInRect(point)) { return &AttriNameStart; }
        else if (CharPropStart.CoordData.PtInRect(point)) { return &CharPropStart; }
        else { return &TreeStart; }
    }

    /// <summary>
    /// Recursively search node banks for matching node coordinates(classic node search code used similar to https://www.codeproject.com/Articles/9887/CStaticTreeCtrl-A-CStatic-derived-custom-Tree-cont).
    /// </summary>
    /// <param name="point">The point.</param>
    /// <param name="pNode">The p node.</param>
    /// <param name="BankType">Type of the bank.</param>
    /// <returns>InfoNode*</returns>
    InfoNode* RecursiveInfoNodeSearch(const CPoint& point, InfoNode* pNode, short BankType)
    {
        if (pNode->CoordData.PtInRect(point))
        {
            return pNode;
        }
        else if (pNode->bOpen)
        {
            InfoNode* ChildNode;
            switch(BankType)//Using Switch to avoid checking type each loop(code same except for bank storage)
            {
            default:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    ChildNode = &this->EventBank[*targetNodeIndex];
                }
                break;
            case 1:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    ChildNode = &this->VariableBank[*targetNodeIndex];
                }
                break;
            case 2:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    ChildNode = &this->AttriNameBank[*targetNodeIndex];
                }
                break;
            case 2:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    ChildNode = &this->CharPropBank[*targetNodeIndex];
                }
                break;
            }
        }
    }

    InfoNode* RetrieveInfoNodeByPoint(CPoint point)
    {
        InfoNode* targetNode = nullptr;
        /*
    RootNode EventDataStart = "Event Data";
    RootNode VariableDataStart = "Variable Data";
    RootNode AttriNameStart = "Attribute Names";
    RootNode CharPropStart = "Character Properties";
        */
        if(NodeSearchRange==0)
        {
            if (EventDataStart.bOpen && point.y > VariableDataStart.CoordData.top){ NodeSearchRange = 0; }
            else if (VariableDataStart.bOpen && point.y > AttriNameStart.CoordData.top) { NodeSearchRange = 1; }
            else if (AttriNameStart.bOpen && point.y > CharPropStart.CoordData.top) { NodeSearchRange = 2; }
            else if (CharPropStart.bOpen) { NodeSearchRange = 3; }
        }
        switch (NodeSearchRange)
        {
        case 0://EventNode NodeTree
            break;
        case 1://VariableNode NodeTree
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
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
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
        if (ToggleNode(point, true)) { CView::OnLButtonUp(nFlags, point); }
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
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
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
