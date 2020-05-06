#pragma once

#include "IniData\CustomOrderedDictionary.h"

//Ordered Custom Dictionary for string keys with unsigned int values
class UIntDic : public CustomOrderedDictionary<std::string, unsigned int>{};