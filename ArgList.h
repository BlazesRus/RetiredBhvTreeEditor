// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once
#ifndef ArgList_IncludeGuard
#define ArgList_IncludeGuard

#include "..\BasicXMLGUI\ArgStringList.h"
//Ordered map from https://github.com/Tessil/ordered-map
#include "tsl\ordered_map.h"

//class ArgElement
//{
//public:
//	ArgStringList Value;
//    ArgElement(std::string value)
//	{
//		Value = value;
//	}
//}

/// <summary>
/// Class named ArgList.
/// Implements the <see cref="tsl::ordered_map{std::string, ValueType}" />
/// </summary>
/// <seealso cref="tsl::ordered_map{std::string, ArgElement}" />
class ArgList : public tsl ::ordered_map<std::string, ArgStringList>
{
public:
    /// <summary>
    /// Use insert if doesn't Already exist, otherwise set the value
    /// </summary>
    /// <param name="Key">The key.</param>
    /// <param name="Value">The value.</param>
    void Add(std::string Key, ArgStringList Value)
    {//https://stackoverflow.com/questions/31792229/how-to-set-a-value-in-an-unordered-map-and-find-out-if-a-new-key-was-added
        auto p = this->insert({ Key, Value });
        if (!p.second) {
            // overwrite previous value
            p.first->second = Value;
        }
    }
    ArgList(){}
    ~ArgList(){}
};
#endif