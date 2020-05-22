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
    COLORREF		m_TagContentColor = RGB(40, 40, 160);
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

    /// <summary>
    /// Nodes bank holding generated Node Linkage order when toggled on(Regenerated each time opened up)
    /// </summary>
    InfoDataDictionary LinkageBank;

    /// <summary>
    /// The linked condition bank
    /// </summary>
    InfoDataDictionary LinkedConditionBank;

    //Places all root-nodes at or below X coordinate of RootEnd
    const long RootEnd = 41;
    /// <summary>
    /// The tree root node
    /// </summary>
    RootNode TreeStart = "Tree Data";
    /// <summary>
    /// Node information for event data root
    /// </summary>
    RootNode EventDataStart = "Event Data";
    /// <summary>
    /// Node information for variable data root
    /// </summary>
    RootNode VariableDataStart = "Variable Data";
    /// <summary>
    /// Node information for attribute name data root
    /// </summary>
    RootNode AttriNameStart = "Attribute Names";
    /// <summary>
    /// Node information for character property data root
    /// </summary>
    RootNode CharPropStart = "Character Properties";
    /// <summary>
    /// Classes Linked together based on calls to other nodes
    /// </summary>
    RootNode LinkageStart = "Node Linkage Order";
    /// <summary>
    /// Condition Classes linked together to other nodes in more descriptive way
    /// </summary>
    RootNode LinkedConditionStart = "Linked Condition Tree";

    /// <summary>
    /// Toggle forces regeneration of linked event class Linked Node children(switch on when variable changed from EventDataStart children) 
    /// </summary>
    bool RegenerateEventLinks = false;
    //Holds data of Name from hkbBehaviorGraphStringData(Event & Variable Names)
    std::string StringDataName = "";
    /// <summary>
    /// Toggle forces regeneration of linked variable class Linked Node children(switch on when variable changed from VariableDataStart children) 
    /// </summary>
    bool RegenerateVariableLinks = false;
    //Holds data of Name from hkbVariableValueSet (Variable Values)
    std::string ValueDataName = "";
    //Holds data of Name from hkbBehaviorGraphData (Variable Types)
    std::string TypeDataName = "";
    //Holds name of variableInitialValues class that stores link to Hk class of type hkbVariableValueSet
    std::string variableInitialValues;
    //Stores link to Hk class of type hkbBehaviorGraphStringData
    std::string stringData;

    /* Don't really need to store information about other rarely edited fields since going to store the full hkclasses instead
    //Holds data of Signature from hkbVariableValueSet (Variable Values)
    std::string ValueDataSignature = "";
    //Holds data of Signature from hkbBehaviorGraphData (Variable Types)
    std::string TypeDataSignature = "";
    //Holds data of Signature from hkbBehaviorGraphStringData(Event & Variable Names)
    std::string StringDataSignature = "";
    //Typically 4 Element Array of what seems to be coordinate positions
    QuadVectorList quadVariableValues;
    */

    //
    /// <summary>
    /// The node type found (0=No Selectable Nodes found;1 = RootNode; 2 = InfoNode; 3 = DataNode)(Node Info for Message Handlers)
    /// </summary>
    short NodeTypeFound = 0;
    /// <summary>
    /// The bank type(0=EventData;1=VariableData;3=AttriNameData;4=CharacterPropertyData;5=NodeLinkageData)
    /// </summary>
    short TargetBankType = 0;
    /// <summary>
    /// The Bank index
    /// </summary>
    unsigned int TargetIndex = 0;
    /// <summary>
    /// The target root node
    /// </summary>
    RootNode* TargetRootNode = nullptr;
    /// <summary>
    /// The target information node
    /// </summary>
    InfoNode* TargetInfoNode = nullptr;
    /// <summary>
    /// The target node
    /// </summary>
    DataNode* TargetNode = nullptr;

    //Index of Node that ClassNodes are within
    unsigned int ClassNodeStart = 0;
    short NodeSearchRange = 0;

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
        const std::string Tab = "    ";
        DataNode* targetNode;
        
        for (UIntVector::iterator CurrentVal = RootNodes.begin(), LastVal = RootNodes.end(); CurrentVal != LastVal; ++CurrentVal)
        {
            targetNode = &NodeBank[*CurrentVal];
            //iDocHeight = RecursivelyDrawNodes(pDC, targetNode, RootEnd, y + rNode.Height(), rFrame);
        }
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

        LinkageBank.clear();
        LinkedConditionBank.clear();
    }

    // Operations
public:
    /// <summary>
    /// Sets the text font.
    /// </summary>
    /// <param name="nHeight">Height of the n.</param>
    /// <param name="bBold">The b bold.</param>
    /// <param name="bItalic">The b italic.</param>
    /// <param name="csFaceName">Name of the font applied</param>
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
    virtual void SetDefaultTextColor(COLORREF textColor)
    {
        m_crDefaultTextColor = textColor;
    }

    /// <summary>
    /// Sets the text settings.
    /// </summary>
    /// <param name="nHeight">Height of the n.</param>
    /// <param name="bBold">The b bold.</param>
    /// <param name="bItalic">The b italic.</param>
    /// <param name="csFaceName">Name of the cs face.</param>
    /// <param name="textColor">The color of the text</param>
    virtual void SetTextSettings(LONG nHeight, BOOL bBold, BOOL bItalic, const CString& csFaceName, COLORREF textColor)
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
        m_crDefaultTextColor = textColor;
    }

    /// <summary>
    /// Toggle node between open and closed
    /// </summary>
    bool ToggleNode(CPoint point, BOOL bInvalidate = FALSE)
    {
        //1=RootNode;2=InfoNode;3=DataNode
        bool FailedToFindNode = false;
        TargetRootNode = nullptr;
        TargetInfoNode = nullptr;
        TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            RetrieveNearestRootNode(point);
            if (TargetRootNode != nullptr) { ToggleNode(TargetRootNode, bInvalidate); FailedToFindNode = false; }
        }
        else
        {
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                RetrieveDataNodeByPoint(point);
                if (TargetNode != nullptr) { ToggleNode(TargetNode, bInvalidate); FailedToFindNode = false; }
            }
            else
            {
                RetrieveInfoNodeByPoint(point);
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
    /// <summary>
    /// Adds all sub nodes.
    /// </summary>
    /// <param name="pNode">The p node.</param>
    /// <param name="TargetNodes">The target nodes.</param>
    void AddAllSubNodes(DataNode* pNode, UIntVector& TargetNodes)
    {
        DataNode* targetNode;
        for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
        {
            targetNode = &this->NodeBank[*targetNodeIndex];
            AddAllSubNodes(targetNode, TargetNodes);
        }
    }

    /// <summary>
    /// Adds all sub nodes.
    /// </summary>
    /// <param name="pNode">The p node.</param>
    /// <param name="TargetNodes">The target nodes.</param>
    /// <param name="BankType">Type of the bank.</param>
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
        case 4:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->LinkageBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 4);
            }
            break;
        case 5:
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->LinkedConditionBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 5);
            }
            break;
        }
    }

    /// <summary>
    /// Deletes the node.
    /// </summary>
    /// <param name="nodeIndex">Index of the node.</param>
    /// <param name="bInvalidate">The b invalidate.</param>
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

    /// <summary>
    /// Deletes the information node.
    /// </summary>
    /// <param name="nodeIndex">Index of the node.</param>
    /// <param name="BankType">Type of the bank.</param>
    /// <param name="bInvalidate">The b invalidate.</param>
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
                AddAllSubNodes(targetNode, TargetNodes, 1);
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
                AddAllSubNodes(targetNode, TargetNodes, 2);
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
                AddAllSubNodes(targetNode, TargetNodes, 3);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                CharPropBank.Remove(*targetNodeIndex);
            }
            break;
        case 4:
            if (pNode->ParentIndex == 0)
            {
                LinkageBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->LinkageBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 4);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                LinkageBank.Remove(*targetNodeIndex);
            }
            break;
        case 5:
            if (pNode->ParentIndex == 0)
            {
                LinkedConditionBank.RootNodes.DeleteFirstMatch(nodeIndex);
            }
            for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                targetNode = &this->LinkedConditionBank[*targetNodeIndex];
                AddAllSubNodes(targetNode, TargetNodes, 5);
            }
            for (UIntVector::iterator targetNodeIndex = TargetNodes.begin(), EndIndex = TargetNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
            {
                LinkedConditionBank.Remove(*targetNodeIndex);
            }
            break;
        }
        if (bInvalidate)
            Invalidate();
    }

    /// <summary>
    /// Draws the nodes from root.
    /// </summary>
    /// <param name="pDC">The p dc.</param>
    /// <param name="x">The x.</param>
    /// <param name="y">The y.</param>
    /// <param name="rFrame">The r frame.</param>
    /// <returns>int.</returns>
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
                iDocHeight = RecursivelyDrawInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 0);
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
                iDocHeight = RecursivelyDrawInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 1);
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
                iDocHeight = RecursivelyDrawInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 2);
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
                iDocHeight = RecursivelyDrawInfoNodes(pDC, targetInfoNode, RootEnd, y + rNode.Height(), rFrame, 3);
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
                iDocHeight = RecursivelyDrawNodes(pDC, targetNode, RootEnd, y + rNode.Height(), rFrame);
            }
        }

        return iDocHeight;
    }

    /// <summary>
    /// Draws the tag content node.
    /// </summary>
    /// <param name="pDC">The p dc.</param>
    /// <param name="pNode">The p node.</param>
    /// <param name="x">The x.</param>
    /// <param name="y">The y.</param>
    /// <param name="rFrame">The r frame.</param>
    /// <param name="iDocHeight">Height of the i document.</param>
    /// <returns>int.</returns>
    template <typename NodeType>
    int DrawTagContentNode(CDC* pDC, NodeType* pNode, int x, int y, CRect rFrame, int iDocHeight)
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
        CString nodeText = pNode->TagName.c_str();
        int		iPos;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, nodeText);
        // Draw only if the node is visible
        if (rNode.bottom > 0 && rNode.top < rFrame.bottom)
        {
            pDC->DrawText(nodeText.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            rNode.left += iPos;
        }
    }

    /// <summary>
    /// Draws single-line non-clickable TagContentNode
    /// </summary>
    /// <param name="pDC">The document pointer.</param>
    /// <param name="pNode">The pointer of target node</param>
    /// <param name="x">X position of node</param>
    /// <param name="y">Y position of node</param>
    /// <param name="rFrame">Frame Coordinates</param>
    /// <param name="iDocHeight">Total document height</param>
    void DrawTagContentInfo(CDC* pDC, CString nodeText, int x, int y, CRect rFrame, int iDocHeight)
    {
        int		iDocHeight = 0;		// Total document height
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        COLORREF crOldText = pDC->SetTextColor(m_crDefaultTextColor);

        int		iPos;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, nodeText);
        // Draw only if the node is visible
        if (rNode.bottom > 0 && rNode.top < rFrame.bottom)
        {
            pDC->DrawText(nodeText.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        }
    }

    /// <summary>
    /// Recursively the draw the InfoNodes
    /// </summary>
    /// <param name="pDC">The document pointer.</param>
    /// <param name="pNode">The pointer of target node</param>
    /// <param name="x">X position of node</param>
    /// <param name="y">Y position of node</param>
    /// <param name="rFrame">Frame Coordinates</param>
    /// <param name="iDocHeight">Total document height</param>
    /// <param name="BankType">NodeBank category(0:EventBank,1:VariableBank)</param>
    /// <returns>int</returns>
    int RecursivelyDrawInfoNodes(CDC* pDC, InfoNode* pNode, int x, int y, CRect rFrame, int iDocHeight, short BankType)
    {
        int		iDocHeight = 0;		// Total document height
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        COLORREF crOldText = pDC->SetTextColor(m_crDefaultTextColor);

        // MULTILINE TEXT - begins
        CString	cs = pNode->TagName.c_str();
        int		iPos;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, cs);
        pDC->DrawText(cs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        pNode->CoordData.CopyRect(rNode);		// Record the rectangle
        pDC->SetTextColor(crOldText);
        if(!pNode->TagContent.empty())
        {
            DrawTagContentInfo(pDC, pNode->TagContent.c_str(), x + iPos, y, rFrame, iDocHeight);
        }
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
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 0);
                }
                break;
            case 1:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->VariableBank[*targetNodeIndex];
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 1);
                }
                break;
            case 2:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->AttriNameBank[*targetNodeIndex];
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 2);
                }
                break;
            case 3:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->CharPropBank[*targetNodeIndex];
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 3);
                }
                break;
            case 4:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->LinkageBank[*targetNodeIndex];
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 4);
                }
                break;
            case 5:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->LinkedConditionBank[*targetNodeIndex];
                    iDocHeight = RecursivelyDrawInfoNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame, m_iDocHeight, 5);
                }
                break;
            }

        }

        return iDocHeight + pNode->CoordData.Height();
    }

    /// <summary>
    /// Draws clickable TagContentNode(Returns X+iPos)
    /// </summary>
    /// <param name="pDC">The document pointer.</param>
    /// <param name="DataContent">Content of the data.</param>
    /// <param name="x">X position of node</param>
    /// <param name="y">Y position of node</param>
    /// <param name="rFrame">Frame Coordinates</param>
    /// <param name="iDocHeight">Total document height</param>
    /// <returns>int</returns>
    int DrawDataContentInfo(CDC* pDC, TagContent& DataContent, int x, int y, CRect rFrame, int& iDocHeight)
    {
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        COLORREF crOldText = pDC->SetTextColor(m_TagContentColor);

        int		iPos;

        // Height of a line of text
        rNode.bottom = rNode.top + m_iLineHeight;

        //------------------Draw primary NodeButton-------------------------------------------------
        // Find out how much text fits in one line
        iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, DataContent.Content);
        // Draw only if the node is visible
        if (rNode.bottom > 0 && rNode.top < rFrame.bottom)
        {
            pDC->DrawText(DataContent.Content.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            DataContent.CoordData.CopyRect(rNode);
        }
        else
        {
            return x;
        }
        return x+iPos;
    }

    /// <summary>
    /// Recursively draws the nodes.
    /// </summary>
    /// <param name="pDC">The document pointer.</param>
    /// <param name="pNode">The pointer of target node</param>
    /// <param name="x">X position of node</param>
    /// <param name="y">Y position of node</param>
    /// <param name="rFrame">Frame Coordinates</param>
    /// <param name="iDocHeight">Total document height</param>
    /// <returns>int</returns>
    int RecursivelyDrawNodes(CDC* pDC, DataNode* pNode, int x, int y, CRect rFrame, int iDocHeight = 0)
    {
        CRect	rNode;

        // The node's location and dimensions on screen
        rNode.left = x;
        rNode.top = y;
        rNode.right = rFrame.right - m_iPadding;
        rNode.bottom = y + m_iLineHeight;

        COLORREF crOldText = pDC->SetTextColor(m_crDefaultTextColor);
        CString nodeTextcs;

        // MULTILINE TEXT - begins
        std::string nodeText = pNode->TagName.c_str();
        std::string TempText;
        int iPos = HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, nodeText.c_str());
        int ArgSize;

        // Height of a line of text(All parts of Node at same height--limiting to single line nodes for now unless need to expand)
        rNode.bottom = rNode.top + m_iLineHeight;

        //-------------------------------------------------------------------------------------------------------

        size_t NumberArgs;
        //for (auto ArgElement : pNode->ArgData)//Draw rest of Nodes based on Args
        for(ArgList::iterator ArgElement = pNode->ArgData.begin(), EndElement = pNode->ArgData.end(); ArgElement != EndElement; ++ArgElement)
        {
            NumberArgs = ArgElement.value.size();
            if (NumberArgs == 0)//Non-Value Element
            {
                TempText = " " + ArgElement.key;
                ArgElement.value.ArgStart = iPos;
                iPos += HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, TempText.c_str());
                nodeText += TempText;
            }
            else if (NumberArgs>1)//Multiple Linked Argument Elements
            {
                TempText = " " + ArgElement.key + "=\"";
                ArgElement.value.ArgStart = iPos;
                iPos += HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, TempText.c_str());
                nodeText += TempText;
                std::vector<LONG>::iterator ArgPos = ArgElement.value.ArgPositions.begin();
                for (ArgStringList::iterator Arg = ArgElement.value.begin(), FirstArg = Arg, LastArg = ArgElement.value.end(); Arg != LastArg; ++Arg)
                {
                    TempText = Arg != FirstArg? *Arg: ", "+ *Arg;
                    //Add Starting position data into ArgStringList
                    *ArgPos = iPos;
                    iPos += HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, TempText.c_str());
                    ++ArgPos;
                }
                TempText += "\"";
                ++iPos;
            }
            else//Single Argument Link
            {
                TempText = " " + ArgElement.key[0]+"=\""+ArgElement.value[0] + "\"";
                ArgElement.value.ArgStart = iPos;
                iPos += HowMuchTextFits(pDC, rFrame.right - m_iPadding - rNode.left, TempText.c_str());
            }
        }
        //Draw only if the node is visible
        if (rNode.bottom > 0 && rNode.top < rFrame.bottom)
        {
            nodeTextcs = nodeText.c_str();
            pDC->DrawText(nodeTextcs.Left(iPos + 1), rNode, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            rNode.left += iPos;
            pNode->CoordData.CopyRect(rNode);		// Record the rectangle
        }
        //Create TagContent as SpecialConnected nodes
        pDC->SetTextColor(crOldText);
        size_t ContentSize = pNode->NodeContent.size();
        if(ContentSize=1)
        {
            DrawDataContentInfo(pDC, pNode->NodeContent[0], x + iPos, y, rFrame, iDocHeight);//Single-Line format TagContent
        }
        else if(ContentSize>1)
        {//Determine how to display by things like node-type, TagName etc
            int XPos = x + iPos;
            
            if(pNode->NodeType==50)//QuadVector format hkparam variable such as(axisOfRotation)
            {
                DrawTagContentInfo(pDC, "(", XPos, y, rFrame, iDocHeight); ++XPos;
                XPos = DrawDataContentInfo(pDC, pNode->NodeContent[0], XPos, y, rFrame, iDocHeight);
                XPos = DrawDataContentInfo(pDC, pNode->NodeContent[1], x + iPos, y, rFrame, iDocHeight);
                XPos = DrawDataContentInfo(pDC, pNode->NodeContent[2], x + iPos, y, rFrame, iDocHeight);
                XPos = DrawDataContentInfo(pDC, pNode->NodeContent[3], x + iPos, y, rFrame, iDocHeight);
                DrawTagContentInfo(pDC, ")", XPos, y, rFrame, iDocHeight);
            }
            else if(pNode->NodeType == 51)//Multi-line QuadVector format
            {
                int XPosStart = XPos;
                int RowPosition = 0;
                DrawTagContentInfo(pDC, "(", XPos, y, rFrame, iDocHeight); ++XPos;
                for (TagContentVector::iterator targetNodeIndex = pNode->NodeContent.begin(), EndIndex = pNode->NodeContent.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    if (RowPosition == 4)
                    {
                        DrawTagContentInfo(pDC, ")", XPos, y, rFrame, iDocHeight);
                        XPos = XPosStart; y = y + pNode->CoordData.Height();
                        iDocHeight += pNode->CoordData.Height();
                        RowPosition = 0;
                        DrawTagContentInfo(pDC, "(", XPos, y, rFrame, iDocHeight);
                    }
                    XPos = DrawDataContentInfo(pDC, *targetNodeIndex, XPos, y, rFrame, iDocHeight);
                    ++RowPosition;
                }
            }
            else
            {
                int XPosStart = XPos;
                int RowPosition = 0;
                for (TagContentVector::iterator targetNodeIndex = pNode->NodeContent.begin(), EndIndex = pNode->NodeContent.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    if(RowPosition==16)//Allow 16 Elements per line
                    {
                        XPos = XPosStart; y = y + pNode->CoordData.Height();
                        iDocHeight += pNode->CoordData.Height();
                        RowPosition = 0;
                    }
                    XPos = DrawDataContentInfo(pDC, *targetNodeIndex, XPos, y, rFrame, iDocHeight);
                    ++RowPosition;
                }
            }
        }
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
                iDocHeight = RecursivelyDrawNodes(pDC, targetNode, x + m_iIndent, y + targetNode->CoordData.Height(), rFrame);
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
		InfoNode* targetInfoNode = nullptr;
		DataNode* targetInfoNode = nullptr;
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
            case 4:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->LinkageBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 4);
                }
                break;
            case 5:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    targetNode = &this->LinkedConditionBank[*targetNodeIndex];
                    RecursivelyDrawLines(pDC, targetNode, 5);
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
            case 4:
                targetNode = &this->LinkageBank[TargetIndex]; break;
            case 5:
                targetNode = &this->LinkedConditionBank[TargetIndex]; break;
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
    /// Retrieves the nearest root node and stores in TargetRootNode.
    /// </summary>
    /// <param name="point">The point.</param>
    void RetrieveNearestRootNode(CPoint point)
    {
        if (EventDataStart.CoordData.PtInRect(point)){ TargetRootNode = &EventDataStart; }
        else if (VariableDataStart.CoordData.PtInRect(point)) { TargetRootNode = &VariableDataStart; }
        else if (AttriNameStart.CoordData.PtInRect(point)) { TargetRootNode = &AttriNameStart; }
        else if (CharPropStart.CoordData.PtInRect(point)) { TargetRootNode = &CharPropStart; }
        else { TargetRootNode = &TreeStart; }
    }

    /// <summary>
    /// Recursively search node banks for matching node coordinates(classic node search code used similar to https://www.codeproject.com/Articles/9887/CStaticTreeCtrl-A-CStatic-derived-custom-Tree-cont).
    /// </summary>
    /// <param name="point">The point.</param>
    /// <param name="pNode">The pointer of target node</param>
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
            case 3:
                for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
                {
                    ChildNode = &this->CharPropBank[*targetNodeIndex];
                }
                break;
            }
        }
    }

    /// <summary>
    /// Retrieves the information node by point (Sets in TargetInfoNode).
    /// </summary>
    /// <param name="point">The point.</param>
    void RetrieveInfoNodeByPoint(CPoint point)
    {
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
    }

    void RetrieveDataNodeByPoint(CPoint point)
    {
	
	}

    void RetrieveNodeByPoint(CPoint point)
    {
        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            TargetRootNode = RetrieveNearestRootNode(point);
			NodeTypeFound = 1;
        }
        else
        {
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                TargetNode = RetrieveDataNodeByPoint(point);
				NodeTypeFound = 3;
            }
            else
            {
                TargetInfoNode = RetrieveInfoNodeByPoint(point);
				NodeTypeFound = 2;
            }
        }
    }

    void AddTagContentNode(unsigned int TargetPIndex)
    {

    }
    // Message handlers
	void OnCM_DeleteNode()
	{
		//DeleteNode(m_pSelected, TRUE);
	}
	void OnCM_ModifyNodeText()
	{
/*		if (NodeTextDlg(m_pSelected->DisplayName) == TRUE)
		{
			Invalidate();
		}*/
	}

	//void OnCM_ToggleConnectingLines()
	//{
	//	m_bShowLines = !m_bShowLines;

	//	Invalidate();
	//}
	//void OnCM_SetConnectingLinesColor()
	//{
	//	CColorDialog ccd(m_crConnectingLines, CC_FULLOPEN | CC_ANYCOLOR);

	//	if (ccd.DoModal() == IDOK)
	//	{
	//		m_crConnectingLines = ccd.GetColor();
	//		Invalidate();
	//	}
	//}
	//void OnCM_SetFont()
	//{
	//	CFontDialog cfd(&m_lgFont, CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT);

	//	if (cfd.DoModal() == IDOK)
	//	{
	//		SetTextFont(cfd.GetSize() / 10, cfd.IsBold(), cfd.IsItalic(), cfd.GetFaceName());
	//		Invalidate();
	//	}
	//}
	//void OnCM_SetDefaultColor()
	//{
	//	CColorDialog ccd(m_crDefaultTextColor, CC_FULLOPEN | CC_ANYCOLOR);

	//	if (ccd.DoModal() == IDOK)
	//	{
	//		m_crDefaultTextColor = ccd.GetColor();
	//		Invalidate();
	//	}
	//}

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
        NodeTypeFound = 0;
        TargetRootNode = nullptr;
        TargetInfoNode = nullptr;
        TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            RetrieveNearestRootNode(point);
            if (TargetRootNode != nullptr) { NodeTypeFound = 1; }
        }
        else
        {
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                RetrieveDataNodeByPoint(point);
                if (TargetNode != nullptr) { NodeTypeFound = 3; }
            }
            else
            {
                RetrieveInfoNodeByPoint(point);
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
        NodeTypeFound = 0;
        TargetRootNode = nullptr;
        TargetInfoNode = nullptr;
        TargetNode = nullptr;

        if (point.x < RootEnd)//Search for RootNode nearest to point
        {
            targetRootNode = RetrieveNearestRootNode(point);
            if (targetRootNode != nullptr) { NodeTypeFound = 1; }
        }
        else
        {
            if (point.y > TreeStart.CoordData.bottom)//Main NodeTree nodes
            {
                TargetNode = RetrieveDataNodeByPoint(point);
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
    //ON_COMMAND(CM_SETCONNECTINGLINESCOLOR, OnCM_SetConnectingLinesColor)
    //ON_COMMAND(CM_SETFONT, OnCM_SetFont)
    //ON_COMMAND(CM_SETDEFAULTCOLOR, OnCM_SetDefaultColor)
END_MESSAGE_MAP()
#endif
