// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once

class ShortcutNode
{
private:

public:
	std::string LinkName;
	
	unsigned int LinkedNodeID;

	/// <summary>
	/// Node Coordinate Data
	/// </summary>
	CRect		CoordData;
	
	ShortcutNode(std::string name, unsigned int id;)
	{
		CoordData.SetRectEmpty();
		LinkName = name;
		LinkedNodeID = id;
	}
};

class ShortcutDatabase : std::vector<ShortcutNode>{};