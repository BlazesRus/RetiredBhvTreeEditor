// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once

#include "OrderedIndexDictionary.h"

/// <summary>
/// Class named InfoNode(derive into InfoNode subclass into view). (Node links to other nodes in node bank)
/// Held within NodeDictionary with int key
/// </summary>
class InfoNode
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
	/// Initializes a new instance of the <see cref="InfoNode"/> class. (All Data Nodes must have display names)
	/// </summary>
	/// <param name="name">The NodeName.</param>
	InfoNode(std::string name)
	{
		TagName = name;
		CoordData.SetRectEmpty();
		bOpen = TRUE;
	}
	
	/// <summary>
	/// Finalizes an instance of the <see cref="InfoNode"/> class.
	/// </summary>
	~InfoNode(){}
};

class InfoDataDictionary : public OrderedIndexDictionary<InfoNode>{};