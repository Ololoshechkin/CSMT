#include "data.h"

const Data Data::kNull = Data();

Data::Data(const std::string& bytes, bool is_empty) : 
	bytes(bytes), is_empty(is_empty) {
}

Data::Data(const std::string& bytes) : Data(bytes, false) {
}

Data::Data() : Data("", false) {
}