#include <iostream>
#include "csmt.h"

int main() {
	auto test = [](const std::vector<int>& order) {
		std::cout << "________________________" << std::endl;
		CSMT csmt;
		for (int x : order) {
			csmt.Insert({x, Data::kNull});
		}
		csmt.Log(std::cout);
		std::cout << "________________________" << std::endl;
	};

	test({1, 3, 7});
	test({3, 1, 7});
	test({7, 3, 1});
	test({7, 1, 3});
	return 0;
}