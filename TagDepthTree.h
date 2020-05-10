// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#ifndef TagDepthTree_Header
#define TagDepthTree_Header

#include <string>
#include <vector>

class TagElement
{
    unsigned int NodeIndex;//Could also easily act as a key to ordered map
    std::string NodeName;
};

/// <summary>
/// Class named TagDepthTree that keeps track of matching tags to keep track of which node(s) to exit).
/// Implements the <see cref="std::vector{TagElement}" />
/// </summary>
/// <seealso cref="std::vector{TagElement}" />
class TagDepthTree : std::vector<TagElement>
{//size_t DepthLevel = 0;

/// <summary>
/// Adds the tag.
/// </summary>
/// <param name="nodeIndex">Index of the node.</param>
/// <param name="nodeName">Name of the node.</param>
    void AddTag(unsigned int nodeIndex, std::string nodeName)
    {

    }

/// <summary>
/// Cycle in reverse to find first match then remove.
/// </summary>
/// <param name="tagName">Name of the tag.</param>
/// <returns>Index of Tag Match</returns>
    int RemoveLastTagMatch(std::string tagName)
    {
        unsigned int nodeIndex;
        return nodeIndex;
    }
};

#endif