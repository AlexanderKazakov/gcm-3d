#include "Utils.h"

bool gcm::Params::has(string param) {
	return find(param) != end();
}

bool gcm::Params::paramEquals(string param, string value) {
	if (has(param))
		if ((*this)[param] == value)
			return true;
	return false;
}

void checkStream(fstream &f) {
	if (f.eof() || f.fail() || f.bad()) 
		THROW_INVALID_INPUT("Input file is corrupted");
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}