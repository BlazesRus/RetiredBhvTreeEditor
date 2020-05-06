#include "StdBhvTree.h"
#include "TreeView.h"

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

    //pNode->rNode.CopyRect(rNode);		// Record the rectangle

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
        return pNode->rNode.Height();
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

    return iDocHeight + pNode->rNode.Height();
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
    int iJointX = pNode->nDisp.TagButton.left - m_iIndent - 6;
    int iJointY = pNode->nDisp.TagButton.top + (m_iLineHeight / 2);
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
