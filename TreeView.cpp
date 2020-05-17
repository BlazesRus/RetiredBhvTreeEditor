#include "StdBhvTree.h"
#include "TreeView.h"
#include "VariableLists/StringVectorList.h"
#include "TagDepthTree.h"

IMPLEMENT_DYNCREATE(TreeView, CView)

void TreeView::OnDraw(CDC* pDC)
{
    MultiViewDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   if (!pDoc)
      return;
    CPaintDC dc(this); // Device context for painting

                       // Double-buffering
    CDC*		pDCMem = new CDC;
    CRect		rFrame;

    GetClientRect(rFrame);

    pDCMem->CreateCompatibleDC(&dc);

    // START DRAW -------------------------------------------------
    pDCMem->FillSolidRect(rFrame, RGB(255, 255, 255));

    UINT	nMode = pDCMem->SetBkMode(TRANSPARENT);
    CFont*	pOldFont = pDCMem->SelectObject(&m_Font);

    int iLastNodePos = 0;
    
    iLastNodePos = DrawNodesFromRoot(pDCMem,  rFrame.left + m_iIndent, m_iPadding - GetScrollPos(SB_VERT), rFrame);
    if (m_bShowLines)
        DrawLinesFromRoot(pDCMem);

    pDCMem->SelectObject(pOldFont);
    pDCMem->SetBkMode(nMode);

    pDCMem->Draw3dRect(rFrame, RGB(0, 0, 0), RGB(0, 0, 0));	// Border

                                                            // END DRAW   -------------------------------------------------

    dc.BitBlt(0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY);

    delete pDCMem;

    // Has the total document height changed?
    if (iLastNodePos != m_iDocHeight)
    {
        BOOL bInvalidate = ((m_iDocHeight < rFrame.Height()) != (iLastNodePos < rFrame.Height()));

        m_iDocHeight = iLastNodePos;

        ResetScrollBar();

        // If the scrollbar has just been hidden/shown, repaint
        if (bInvalidate)
            Invalidate();
    }
}

/// <summary>
/// Loads the data from file.
/// </summary>
/// <param name="FilePath">The file path.</param>
/// <returns>bool</returns>
bool TreeView::LoadDataFromFile(std::string FilePath)
{
    Reset();
    char LineChar;
    bool InsideXMLComment = false;
    //If false, then inside tagcontent types instead of tags
    bool InsideTag = false;
    std::string ScanBuffer = "";
    
    //First name inside tag becomes CurrentTag
    std::string CurrentTag = "";
    std::string CurrentNodeName = "";
	unsigned int CurrentNodeIndex = 0;
     //0=NormalTag; 1:SelfContainedTag; 2:TagIsClosing; 3:XMLVersionTag
    int TagType = 0;
	
	bool ScanningArgData = false;
	std::string ArgElement;
    ArgStringList LastArg;
    ArgList ArgBuffer;

    bool PotentialComment = false;
    bool InsideParenthesis = false;
    bool InsideClassNodeSection = false;
	//0 = Not Scanning TagContent Yet: 1 = Potential SingleLine TagContent: 2 Multi-line target content
	short TagContentStage = 0;

    //Current state of code loading for certain sections of code
    size_t Stage = 0;

	//Index of hkobject class node
    unsigned int CurrentClassNodeIndex = 0;
	//Index of last entered node
    unsigned int CurrentNodeIndex = 0;
    TagDepthTree TagDepth;
	std::string HkClassType = "";

    //std::string CurrentNodeName = "";
	
	
	unsigned ClassNodeIndex = 0;
    //class parameter of hkobject

    //BhvTreeViewNode* TargetNode = nullptr;
    const std::string hkobject = "hkobject";
    const std::string Signature = "Signature";

    std::ifstream inFile;
    inFile.open(FilePath);
    if (!inFile)
    {
        return false;
    }
    while (inFile >> LineChar)
    {
        if (PotentialComment)
        {
            ScanBuffer += LineChar;
            if (ScanBuffer == "--")
            {
                InsideXMLComment = true;
                PotentialComment = false;
                ScanBuffer = "";
            }
            else if (ScanBuffer.size() >= 2)//Detecting non-normal format TagName?
            {
                PotentialComment = false;
                ScanBuffer = "!" + ScanBuffer;
            }
        }
        else if (InsideXMLComment)//Ignoring all xml inside xml formatted comment
        {
            Stage = ScanBuffer.size();
            if (Stage == 0)
            {
                if (LineChar == '-')
                {
                    ScanBuffer = "-";
                }
            }
            else if (Stage == 1)
            {
                if (LineChar == '-')
                {
                    ScanBuffer = "--";
                }
                else
                {
                    ScanBuffer = "";
                }
            }
            else
            {
                if (LineChar == '>')
                {
                    InsideXMLComment = false;
                }
                ScanBuffer = "";
            }
        }
		else if (ScanningArgData)
        {
            if (Stage == 0)
            {
                if (LineChar == '=')
                {
                    Stage = 1;
                    //CurrentArgName = ScanBuffer;//Don't need to store in separate variable since ScanBuffer not going to be changed until exit scanning argument data
                    ArgElement = "";
                }
                else if (LineChar != ' ' && LineChar != '\t' && LineChar != '\n')//Skip Whitespace
                {
                    ScanBuffer += LineChar;
                }
            }
            else if(Stage==1)
            {
                if (LineChar == '\"')
                {
                    Stage = 2;
                }
            }
			else if(Stage==2)
			{
				if(LineChar=='\"'){	Stage = 3; }
			}
            else if(Stage==3)
            {
                if (LineChar == ',')
                {
                    LastArg.push_back(ArgElement);
                }
                else if (LineChar == '\"')
                {
                    ArgBuffer.Add(ScanBuffer, LastArg);
                    ScanningArgData = false;
                }
                else
                {
                    ArgElement += LineChar;
                }
            }
        }
		else
		{
            if (InsideTag)
            {
				if(ScanBuffer=="/"&&LineChar=='>')
				{
				    switch(TagType)
					{
					default://TagIsClosing(TagType==2)
                        //Decrease TagDepth
                        TagDepth.RemoveLastTagMatch(CurrentTag);
						break;
					case 1://SelfContainedTag
                        NodeBank.Add(CurrentTag, ArgBuffer, CurrentNodeIndex);
                        break;
					case 3://XMLVersionTag(Same as SelfContained Tag except for ? in front and such)
						break;
					}
					CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
					InsideTag = false;TagContentStage = 0;
				}
                else if (LineChar =='>')
                {
				    //(TagType==0)
					CurrentNodeIndex = NodeBank.Add(CurrentTag, ArgBuffer, CurrentNodeIndex);//Index of Last Entered Node is it's parent
					if(InsideClassNodeSection&&CurrentClassNodeIndex==0&&CurrentTag==hkobject&&ArgBuffer.HasKey(Signature))
					{
						CurrentClassNodeIndex = CurrentNodeIndex;
                        CurrentNodeName = ArgBuffer["Name"][0];
						NodeLinks.Add(CurrentNodeName,  CurrentClassNodeIndex);
					}
					//Increase TagDepth
					CurrentNodeName = CurrentTag;
					CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
					InsideTag = false;TagContentStage = 0;
					
                }
                else if(CurrentTag.empty())
                {
					if(!ScanBuffer.empty())
					{
						if(LineChar=='!')//Detecting potential Commented Out Parts
						{
							PotentialComment = true;
						}
						else if(LineChar==" "|| LineChar=="	")
						{
							CurrentTag = ScanBuffer;
						}
						else
						{
							ScanBuffer += LineChar;
						}
				}
				else if(LineChar!=' '&& LineChar!='	' && LineChar!='\n')
				{
					ScanBuffer += LineChar;
					if(LineChar!='\\')
					{
						ScanningArgData=true;Stage=0;
					}
				}
			}
			else if(TagContentStage>0)
			{
			
			}
			else
			{
                if (LineChar == '<')
                {
                    InsideTag = true;
				}
				else if(LineChar!=' '&& LineChar!='	' && LineChar!='\n')
				{
					ScanBuffer = LineChar;
					TagContentStage = 1;
				}
			}
		}
	}
    return true;
}


void TreeView::SaveDataToFile(std::string FilePath)
{

}

//BankTypes 0=Event; 1=Variable
inline int TreeView::DrawRecursiveInfoNodes(CDC* pDC, InfoNode* pNode, int x, int y, CRect rFrame, short BankType)
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
        switch(BankType)
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

inline int TreeView::DrawRecursiveNodes(CDC* pDC, DataNode* pNode, int x, int y, CRect rFrame)
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

inline void TreeView::DrawLinesRecursive(CDC* pDC, DataNode* pNode)
{
    DataNode* targetNode = nullptr;
    if (pNode->bOpen)
    {
        for (UIntVector::iterator targetNodeIndex = pNode->ChildNodes.begin(), EndIndex = pNode->ChildNodes.end(); targetNodeIndex != EndIndex; ++targetNodeIndex)
        {
            targetNode = &this->NodeBank[*targetNodeIndex];
            DrawLinesRecursive(pDC, targetNode);
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

void TreeView::SaveDataToFile(std::string FilePath)
{

}
#ifdef _DEBUG
void TreeView::AssertValid() const
{
    CView::AssertValid();
}

void TreeView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG

MultiViewDoc* TreeView::GetDocument()
{
    return (MultiViewDoc*)m_pDocument;
}