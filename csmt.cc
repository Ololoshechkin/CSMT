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
	while ((1 << (deg + 1) < x)) ++deg;
	return deg;
}

int distance(int x, int y) {
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

std::unique_ptr<CSMT::Node> UpdateNode(std::unique_ptr<CSMT::Node>& node) {
	if (!node) return std::move(node);
	return std::move(node = MakeNode(node->left, node->right));
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
	auto l_dist = distance(kvpair.key, GetKey(node->left));
	auto r_dist = distance(kvpair.key, GetKey(node->right));
	if (l_dist == r_dist) {
		auto new_leaf = std::make_unique<CSMT::Node>(kvpair);
		auto min_key = std::min(GetKey(node->left), GetKey(node->right));
		if (kvpair.key < min_key) {
			return MakeNode(new_leaf, node);
		} else {
			return MakeNode(node, new_leaf);
		}
	}
	if (l_dist < r_dist) {
		node->left = InsertImpl(node->left, kvpair);
		return UpdateNode(node);
	}
	if (l_dist > r_dist) {
		node->right = InsertImpl(node->right, kvpair);
		return UpdateNode(node);
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
	std::function<void(const std::unique_ptr<CSMT::Node>& node, int)> log_impl =
	[&out, &log_impl](const std::unique_ptr<CSMT::Node>& node, int depth) {
		if (!node) return;
		for (int i = 0; i < depth; ++i) out << ' ';
		out << GetKey(node) << std::endl;
		log_impl(node->left, depth + 1);
		log_impl(node->right, depth + 1);
	};
	log_impl(root, 0);
}

void CSMT::Delete(int key) {}

std::vector<Data> CSMT::GetProof(int key) const { return {}; }
