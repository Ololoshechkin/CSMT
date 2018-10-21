#pragma once

#include <string>

struct Data {
	static const Data kNull;

	Data();
	Data(const std::string& bytes);
	Data(const std::string& bytes, bool is_empty);

	std::string bytes;
	bool is_empty;
};