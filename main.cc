#include <iostream>
#include <memory>
#include "csmt.h"

template <typename N>
int Key(const N& n) {
	return n ? n->kvpair.key : -1;
}

int main() {
	auto test = [](const std::vector<int>& order, const std::vector<int>& delete_order) {
		std::cout << "________________________" << std::endl;
		CSMT csmt;
		for (int x : order) {
			csmt.Insert({x, Data::kNull});
		}
		for (int x : delete_order) {
			csmt.Delete(x);
		}
		csmt.Dfs([](const std::unique_ptr<CSMT::Node>& node) {
			if (Key(node->left) > Key(node->right)) {
				std::cout << " EEEEEEEEE : " << Key(node->left) << ' ' << Key(node->right) << std::endl;
			}
		}, [] {}, [] {});
		csmt.Log(std::cout);
		std::cout << "________________________" << std::endl;
	};
	test({1, 123, 15, 123, 51, 6, 161, 9999, 16, 716, 8, 9}, {9999});
	test({161, 1, 123, 51, 123, 6, 9, 16, 716, 8, 15}, {});
	return 0;
}