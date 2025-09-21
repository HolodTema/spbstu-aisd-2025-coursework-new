#include "exceptions.h"

const char* UnableToParseKeyFileException::what() const noexcept {
	return "Error: unable to parse key-file.\n";
}

const char* UnableToOpenEncodedTextFileException::what() const noexcept {
	return "Error: unable to open file with encoded text to decode it.\n";
}
