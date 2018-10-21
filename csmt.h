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

	template <typename F, typename G, typename H>
	void Dfs(F node_process, G child_enter, H child_exit) {
		std::function<void(const std::unique_ptr<CSMT::Node>& node)> dfs_impl =
		[&](const std::unique_ptr<CSMT::Node>& node) {
			if (!node) return;
			node_process(node);
			child_enter();
			dfs_impl(node->left);
			child_exit();
			child_enter();
			dfs_impl(node->right);
			child_exit();
		};
		dfs_impl(root);
	}

  private:

	std::unique_ptr<Node> root;
};