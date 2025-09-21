#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP

#include <string>
#include "EncodingKey.hpp"
#include "DecodingKey.hpp"

std::wstring readFileToEncode(const std::string& filePath);

std::string readEncodedFileDebugMode(const std::string& filePath);

std::string readEncodedFileBinaryMode(const std::string& filePath);

DecodingKey parseKeyFile(const std::string& filePath);

void saveKeyFile(const EncodingKey& encodingKey);

void saveEncodedFileDebugMode(const std::string& encodedText);

void saveEncodedFileBinaryMode(std::string& encodedText, const int& residualZeroes);

void saveDecodedFile(const std::wstring& text);

#endif
