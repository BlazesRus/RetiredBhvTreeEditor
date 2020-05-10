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
    int NodeIndex;
    std::string NodeName;
};

class TagDepthTree : std::vector<TagElement>
{
    //size_t DepthLevel = 0;
};

#endif