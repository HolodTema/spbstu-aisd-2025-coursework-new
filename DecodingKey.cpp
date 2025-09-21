#include "DecodingKey.hpp"
#include <limits>
#include "config.h"

//todo fis is fail() without fail..
DecodingKey::DecodingKey(std::istream& is):
	mapCodes_(1),
	residualZeroes_(0)
{
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	is >> residualZeroes_;
	if (!is) {
		throw std::runtime_error("Error: unable to read key-file.");
	}
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	size_t mapCodesSize = 0;
	is >> mapCodesSize;
	is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (!is) {
		throw std::runtime_error("Error: unable to read key-file.");
	}
	mapCodes_ = HashMap<std::string, wchar_t>(mapCodesSize);
	std::string line;
	const char* delim = " ";
	while (std::getline(is, line)) {
		unsigned int charOrd = 0;
		try {
			charOrd = std::stoi(line.substr(0, line.find(delim)));
		}
		catch (const std::invalid_argument&) {
			throw std::runtime_error("error: unable to read key-file.");
		}
		std::string charCode = line.substr(line.find(delim) + 1);
		mapCodes_.insert(charCode, static_cast<wchar_t>(charOrd));
		if (!is) {
			throw std::runtime_error("Error: unable to read key-file.");
		}
	}
}

HashMap<std::string, wchar_t> DecodingKey::getMapCodes() const {
	return mapCodes_;
}

int DecodingKey::getResidualZeroes() const {
	return residualZeroes_;
}

