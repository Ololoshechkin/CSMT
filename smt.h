#pragma once

#include <vector>

#include "data.h"

struct KVPair {
	int key;
	Data value;
};

class SMT {
  public:
  	virtual ~SMT() = default;

	virtual void Insert(const KVPair& kvpair) = 0;

	virtual void Delete(int key) = 0;

	virtual std::vector<Data> GetProof(int key) const = 0;
};