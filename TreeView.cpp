#include "StdBhvTree.h"
#include "TreeView.h"
#include "GlobalCode_VariableLists/StringVectorList.h"

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
	
	//std::string ContentBuffer = "";
	
	//First name inside tag becomes CurrentTag
	std::string CurrentTag = "";
	//int CurrentTagIndex = -1;//Default to empty node
	std::string ArgName = "";
	std::string ArgValue = "";

	 //0=NormalTag; 1:SelfContainedTag; 2:TagIsClosing; 3:XMLVersionTag
	int TagType = 0;
	//bool ArgHasNoValue = false;
	bool PotentialComment = false;
	bool InsideParenthesis = false;
	bool ScanningArgData = false;
	bool InsideClassNodeSection = false;
	
	//bool TagNameHasArg02 = false;
	//std::string TagNameArg02 = "";
	
	//Used for storing size or current state of code loading
	//Stage 999 = Potential entry point of hkobject tag(Not inside HKclassobject yet)
	size_t StageOrSize = 0;
	size_t DepthLevel = 0;

	//bool InsideActionClass;
	std::string CurrentNodeName = "";
	//class parameter of hkobject
	std::string HkClassType = "";
	//Signature parameter of hkobject
	std::string SingTemp = "";
	//BhvTreeViewNode* TargetNode = nullptr;

	ifstream inFile;
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
			StageOrSize = ScanBuffer.size();
			if (StageOrSize == 0)
			{
				if (LineChar == '-')
				{
					ScanBuffer = "-";
				}
			}
			else if (StageOrSize == 1)
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
/*		else if (ScanningArgData)
		{
			if (StageOrSize == 0)
			{
				if (LineChar != '=')
				{
					StageOrSize = 1; TagNameArg02 = "";
				}
				else if (LineChar != ' ' && LineChar != '\t' && LineChar != '\n')//Skip Whitespace
				{
					ScanBuffer += LineChar;
				}
			}
			else
			{
				if (LineChar == '\"')
				{
					if (InsideParenthesis && !TagNameArg02.empty())//End argument inside parenthesis with second parenthesis instead of space
					{
						////Detect Argument type(0=Default/String; 1:Int; 2:Non-WholeNumber)
						//int DetectedArgType = 0;
						//for(int Index=0;Index<TagNameArg02.length();++Index)
						//{

						//}
						//switch(DetectedArgType)
						//{
						//	case 2:
						//		additionTagOptions.Add(ScanBuffer, TagNameArg02);
						//	break;
						//	default:
						additionTagOptions.Add(ScanBuffer, TagNameArg02);
						//	break;
						//}
					}
					InsideParenthesis = !InsideParenthesis;
				}
				else
				{
					if (InsideParenthesis)
					{
						TagNameArg02 += LineChar;
					}
				}
			}
		}*/
		else if(CurrentNodeName==""||StageOrSize==999)//Not inside HKclassobject yet
		{
			if (InsideTag)
			{
				if (LineChar == '>')
				{
					if(CurrentTag=="hkobject")
					{
						NodeBank.Add(CurrentNodeName, HkClassType, SingTemp);
						DepthLevel = 0;//Exit once closing hkobject tag while DepthLevel = 0
						InsideTag = false;
					}
				}
				else if(!ScanBuffer.empty())
				{
					if(LineChar=='!')//Detecting potential Commented Out Parts
					{
						PotentialComment = true;
					}
					else if(LineChar=" "|| LineChar="	")
					{
						CurrentTag = ScanBuffer;
						if(CurrentTag=="hkobject")//(Assuming inside subnode of <hksection name="__data__"> tag)
						{
						
						}
						else if(CurrentTag=="?xml")
						{
						
						}
						else if(CurrentTag!="hksection")//(Skipping information of about which section reading data on)
						{
						}
					}
				}
				else
				{
					ScanBuffer += LineChar;
				}
			}
			else
			{
				if (LineChar == '<')//Don't worry about tagcontent yet
				{
					InsideTag = true;
					StageOrSize = 999;//
				}
			}
		}/*
		else if(HkClassType=="hkbBehaviorGraphStringData")//Setting names
		{
		
		}
		else if(HkClassType=="hkbVariableValueSet")//Setting Variable Values
		{
		
		}
		else if(HkClassType=="hkbBehaviorGraphData")//Setting Role values
		{
		
		}*/
		//else if(HkClassType=="hkbBehaviorGraph"){}
		else//Assuming inside an hkobject with name of CurrentNodeName
		{		
			if (InsideTag)
			{
				if (LineChar == '>')
				{
				}
			}
			else if (LineChar == '<'){	InsideTag = true; }
		}
	}
	return true;
}


void TreeView::SaveDataToFile(std::string FilePath)
{

}

inline int TreeView::DrawNodesRecursive(CDC* pDC, DataNode* pNode, int x, int y, CRect rFrame)
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
            iDocHeight = DrawNodesRecursive(pDC, targetNode, x + m_iIndent, y + targetNode->rNode.Height(), rFrame);
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
        targetNode = this->NodeBank[TargetIndex];
        // How far up from the joint is the parent?
        int iDispY = iJointY - targetNode->rNode.top - (m_iLineHeight / 2);

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