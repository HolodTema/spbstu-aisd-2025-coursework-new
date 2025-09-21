#include "EncodingKey.hpp"

#include <limits>

#include "config.h"

HashMap<wchar_t, std::string> EncodingKey::getMapCodes() const {
	return mapCodes_;
}

int EncodingKey::getResidualZeroes() const {
	return residualZeroes_;
}

std::ostream &operator<<(std::ostream &os, const EncodingKey &encodingKey) {
	std::ostream::sentry sentry(os);
	if (!sentry) {
		return os;
	}

	os << "Key-file for Shannon-Fano encoding. Version " << PROGRAM_VERSION << "\n";
	os << encodingKey.residualZeroes_ << "\n";
	os << encodingKey.mapCodes_.amountElements() << "\n";

	auto iterationCallback = [&os](const HashMap<wchar_t, std::string>::Pair& pair) {
		unsigned int ord = static_cast<unsigned int>(pair.key);
		os << std::to_string(ord) << " " << pair.value << "\n";
		return true;
	};
	encodingKey.mapCodes_.forEach(iterationCallback);
	return os;
}