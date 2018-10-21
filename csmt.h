#pragma once

#include <memory>
#include <ostream>

#include "smt.h"

class CSMT : public SMT {
  public:
  	struct Node {
		Node(const KVPair& kvpair, std::unique_ptr<Node> left = nullptr,
			std::unique_ptr<Node> right = nullptr);

		bool IsLeaf() const;

		KVPair kvpair;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
	};

	void Insert(const KVPair& kvpair) override;

	void Delete(int key) override;

	std::vector<Data> GetProof(int key) const override;

	void Log(std::ostream& out);

  private:

	std::unique_ptr<Node> root;
};