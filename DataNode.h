// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#if !defined(BasicDataNode_IncludeGuard)
#define BasicDataNode_IncludeGuard

#include "ArgList.h"
#include "..\BasicXMLGUI\UIntVector.h"

/// <summary>
/// Class named BasicDataNode(derive into DataNode subclass into view). (Node links to other nodes in node bank)
/// Held within NodeDictionary with int key
/// </summary>
class DataNode
{public:
	/// <summary>
	/// Index position of ParentNode (EmptyNode=inside Root Level by default)
	/// </summary>
	int ParentIndex;
	
	/// <summary>
	/// The indexes of all child nodes
	/// </summary>
	UIntVector ChildNodes;

	std::string TagName;
	
	ArgList ArgData;
    //0 = Default
	//-------Arg Data Stored as Separate nodes-------------------
	//-------90 = Arg Field
	//91 = Arg Element
    //92 = EventNode Link(From ArgData)
    //93 = VariableNode Link(From ArgData)
	//94 = ClassNode Link(From ArgData)
    //95 = Character Property Link(From ArgData)
	//96 = Attribute Name Link(From ArgData)
	//------Any at or above 100 reserved for TagContent------
	//100 = SingleLine TagContent
	//101 = SingleLine Event Target
	//102 = SingleLine Variable Target
	//103 = SingleLine ClassNode Target
	//104 = SingleLine Character Property Target
	//105 = SingleLine Attribute Name Target
	//------------------------------------------------------------------------
	//110 = Multiline TagContentElement
    //111 = Multiline Event Target
    //112 = Multiline Variable Target
    //113 = Multiline ClassNode Target
	//114 = Multiline Character Property Target
	//115 = Multiline Attribute Name Target
    char NodeType;

	/// <summary>
	/// The b open
	/// </summary>
	BOOL    bOpen;
	
	/// <summary>
	/// Node Coordinate Data(Current Position of Node in Tree)
	/// </summary>
	CRect		CoordData;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="DataNode"/> class. (All Data Nodes must have display names)
	/// </summary>
	/// <param name="name">The NodeName.</param>
	DataNode(std::string name)
	{
		TagName = name;
		CoordData.SetRectEmpty();
		bOpen = FALSE;
		NodeType = 0;
		ParentIndex = 0;
	}
	
	/// <summary>
	/// Finalizes an instance of the <see cref="DataNode"/> class.
	/// </summary>
	~DataNode(){}
};
#endif