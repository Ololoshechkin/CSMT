#include <memory>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <functional>

#include "csmt.h"
#include "sha256.h"

namespace {

int log2(int x) {
	int deg = 0;
	while (((1 << (deg + 1)) <= x)) ++deg;
	return deg;
}

int Distance(int x, int y) {
	return log2(x ^ y);
}

int GetKey(const std::unique_ptr<CSMT::Node>& node) {
	return node ? node->kvpair.key : -1;
}

Data GetValue(const std::unique_ptr<CSMT::Node>& node) {
	return node ? node->kvpair.value : Data::kNull;
}

std::unique_ptr<CSMT::Node> MakeNode(std::unique_ptr<CSMT::Node>& left,
                                     std::unique_ptr<CSMT::Node>& right) {
	auto key = std::max(GetKey(left), GetKey(right));
	auto value = MergedHash(GetValue(left), GetValue(right));
	return std::make_unique<CSMT::Node>(KVPair{key, value}, std::move(left), std::move(right));
}

std::unique_ptr<CSMT::Node>& UpdateNode(std::unique_ptr<CSMT::Node>& node) {
	if (!node) return node;
	return node = MakeNode(node->left, node->right);
}

std::unique_ptr<CSMT::Node> InsertImpl(std::unique_ptr<CSMT::Node>& node,
                                       const KVPair& kvpair) {
	if (node == nullptr) {
		return std::make_unique<CSMT::Node>(kvpair);
	}
	if (node->IsLeaf()) {
		auto new_leaf = std::make_unique<CSMT::Node>(kvpair);
		if (kvpair.key == GetKey(node)) {
			std::cerr << "key exists" << std::endl;
			return std::move(node);
		} else if (kvpair.key > GetKey(node)) {
			return MakeNode(node, new_leaf);
		} else {
			return MakeNode(new_leaf, node);
		}
	}
	auto l_dist = Distance(kvpair.key, GetKey(node->left));
	auto r_dist = Distance(kvpair.key, GetKey(node->right));
	if (l_dist == r_dist) {
		auto new_leaf = std::make_unique<CSMT::Node>(kvpair);
		auto min_key = std::min(GetKey(node->left), GetKey(node->right));
		if (kvpair.key < min_key) {
			return MakeNode(new_leaf, node);
		} else {
			return MakeNode(node, new_leaf);
		}
	} else if (l_dist < r_dist) {
		node->left = InsertImpl(node->left, kvpair);
		return std::move(UpdateNode(node));
	} else {
		node->right = InsertImpl(node->right, kvpair);
		return std::move(UpdateNode(node));
	}
}

bool CheckForLeaf(std::unique_ptr<CSMT::Node>& left,
                  std::unique_ptr<CSMT::Node>& right, int key) {
	return (left->IsLeaf() && GetKey(left) == key) || (right->IsLeaf() && GetKey(right) == key);
}

std::unique_ptr<CSMT::Node> DeleteImpl(std::unique_ptr<CSMT::Node>& node,
                                       int key) {
	if (CheckForLeaf(node->left, node->right, key)) {
		if (GetKey(node->left) == key) {
			node = std::move(node->right);
			return std::move(node);
		} else {
			node = std::move(node->left);
			return std::move(node);
		}
	} else {
		auto l_dist = Distance(key, GetKey(node->left));
		auto r_dist = Distance(key, GetKey(node->right));
		if (l_dist == r_dist) {
			std::cerr << "key does not exist" << std::endl;
			return std::move(node);
		} else if (l_dist < r_dist) {
			node->left = DeleteImpl(node->left, key);
			return MakeNode(node->left, node->right);
		} else {
			node->right = DeleteImpl(node->right, key);
			return MakeNode(node->left, node->right);
		}
	}
}

} // namespace

CSMT::Node::Node(const KVPair& kvpair, std::unique_ptr<CSMT::Node> left,
                 std::unique_ptr<CSMT::Node> right) :
	kvpair(kvpair), left(std::move(left)), right(std::move(right)) {
}

bool CSMT::Node::IsLeaf() const {
	return !left && !right;
}

void CSMT::Insert(const KVPair& kvpair) {
	root = InsertImpl(root, kvpair);
}

void CSMT::Log(std::ostream& out) {
	int depth = 0;
	Dfs([&out, &depth](const std::unique_ptr<Node>& node) {
		for (int i = 0; i < depth; ++i) out << ' ';
		out << GetKey(node) << std::endl;
	}, [&depth]() { depth++; }, [&depth]() { depth--; });
}

void CSMT::Delete(int key) {
	root = DeleteImpl(root, key);
}

std::vector<Data> CSMT::GetProof(int key) const { return {}; }
