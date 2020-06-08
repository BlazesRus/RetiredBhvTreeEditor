// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#if !defined(HighlightedNode_IncludeGuard)
#define HighlightedNode_IncludeGuard

#include "OrderedIndexDictionary.h"

/// <summary>
/// Class HightlightedNode.
/// </summary>
class HightlightedNode
{public:
    /// <summary>
    /// Index position of ParentNode (EmptyNode=inside Root Level by default)
    /// </summary>
    int ParentIndex;

    //std::string DisplayName;

    /// <summary>
    /// The linked node index(Actual Stored data inside another dictionary)
    /// </summary>
    unsigned int LinkedNodeIndex;

    short LinkedDictionaryIndex;
};

/// <summary>
/// Class HighlightedNodeDictionary.
/// Implements the <see cref="OrderedIndexDictionary{HightlightedNode}" />
/// </summary>
/// <seealso cref="OrderedIndexDictionary{HightlightedNode}" />
class HighlightedNodeDictionary : public OrderedIndexDictionary<HightlightedNode>
{

};

#endif