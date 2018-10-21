#pragma once

#include <string>

#include "data.h"

Data Hash(const Data& data);

Data MergedHash(const Data& left, const Data& right);