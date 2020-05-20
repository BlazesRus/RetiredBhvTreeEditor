// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#if !defined(TagContentVector_IncludeGuard)
#define TagContentVector_IncludeGuard

#include <vector>
#include <string>
#include "StdBhvTree.h"

/// <summary>
/// Class TagContent.
/// </summary>
class TagContent
{
public:
	/// <summary>
	/// Actual Tag Content Data to display
	/// </summary>
	CString Content;
	/// <summary>
	/// Node Coordinate Data(Current Position of Node in Tree)
	/// </summary>
	CRect		CoordData;

	/// <summary>
	/// Initializes a new instance of the <see cref="TagContent"/> class.
	/// </summary>
	TagContent()
	{
		Content = "";
		CoordData.SetRectEmpty();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="TagContent"/> class.
	/// </summary>
	/// <param name="Value">The value.</param>
	TagContent(CString Value)
    {
        Content = Value;
        CoordData.SetRectEmpty();
    }

	/// <summary>
	/// Initializes a new instance of the <see cref="TagContent"/> class.
	/// </summary>
	/// <param name="Value">The value.</param>
	TagContent(std::string Value)
    {
        Content = Value.c_str();
        CoordData.SetRectEmpty();
    }
};

/// <summary>
/// Shortcut to prevent need to construct new derived Vectors for Unsigned Int
/// </summary>
/// <seealso cref="std::vector" />
class TagContentVector : public std::vector<TagContent>
{
public:

/*
	/// <summary>
	/// Deletes the first match.
	/// </summary>
	/// <param name="targetVal">The target value.</param>
	/// <returns>bool.</returns>
	bool DeleteFirstMatch(unsigned int targetVal)
	{
		bool NodeFound = false;
		for (TagContentVector::iterator CurrentVal = this->begin(), LastVal = this->end(); NodeFound==false && CurrentVal != LastVal; ++CurrentVal)
		{
			if (*CurrentVal == targetVal)
			{
				NodeFound = true;
				this->erase(CurrentVal);
			}
		}
		return NodeFound;
	}
*/
};

#endif