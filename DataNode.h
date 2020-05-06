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
    //1 = EventNode
    //2 = VariableNode
    //3 = Character Property
    //11 = EventNode Link
    //12 = VariableNode Link
    //13 = Character Property Link
	//Any at or above 100 reserved for TagContent
	//100 = SingleLine TagContent
	//110 = Multiline TagContentElement
	//101 = SingleLine Event Target
	//102 = SingleLine Variable Target
	//103 = SingleLine ClassNode Target
    //111 = Multiline Event Target
    //112 = Multiline Variable Target
    //113 = Multiline ClassNode Target
    char NodeType;

	/// <summary>
	/// The b open
	/// </summary>
	BOOL    bOpen;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="DataNode"/> class. (All Data Nodes must have display names)
	/// </summary>
	/// <param name="name">The NodeName.</param>
	DataNode(std::string name)
	{
		TagName = name;
		//rNode.SetRectEmpty();
		bOpen = TRUE;
		NodeType = 0;
	}
	
	/// <summary>
	/// Finalizes an instance of the <see cref="DataNode"/> class.
	/// </summary>
	~DataNode(){}
};
#endif