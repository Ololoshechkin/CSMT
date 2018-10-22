#include <iostream>
#include <memory>
#include <set>
#include "csmt.h"

template <typename N>
int Key(const N& n) {
	return n ? n->kvpair.key : -1;
}

int main() {
	auto test = [](const std::vector<int>& order, const std::vector<int>& delete_order) {
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
	};
	for (int i = 0; i < 10000; ++i) {
		std::cout << i << std::endl;
		std::vector<int> keys(1000 + (rand() % 25));
		std::set<int> unique_keys;
		for (int i = 0; i < keys.size(); ++i) {
			int key = 1;
			while (true) {
				key = 1 + rand() % 2000;
				if (!unique_keys.count(key)) break;
			}
			unique_keys.insert(key);
			keys[i] = key;
		}
		test(keys, {});
	}
	return 0;
}