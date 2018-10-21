#include "sha256.h"

// TODO : impl this.
Data Hash(const Data& data) { return data; }

Data MergedHash(const Data& left, const Data& right) { 
	if (left.is_empty && right.is_empty) return Data::kNull;
	return left.is_empty ? right : left;
}