// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#if !defined(RootNode_IncludeGuard)
#define RootNode_IncludeGuard

/// <summary>
/// Class named RootNode that holds  Position Data and open status for TreeData
/// </summary>
class RootNode
{public:
	std::string NodeName;

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
	/// Initializes a new instance of the <see cref="RootNode"/> class. (All Data Nodes must have display names)
	/// </summary>
	/// <param name="name">The NodeName.</param>
	RootNode(std::string name)
	{
		NodeName = name;
		CoordData.SetRectEmpty();
		bOpen = FALSE;
	}
	
	/// <summary>
	/// Finalizes an instance of the <see cref="RootNode"/> class.
	/// </summary>
	~RootNode(){}
};
#endif