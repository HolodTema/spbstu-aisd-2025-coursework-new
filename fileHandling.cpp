#include "fileHandling.hpp"
#include <bitset>
#include <codecvt>
#include <fstream>
#include "config.h"

std::wstring readFileToEncode(const std::string& filePath) {
	std::wifstream in(filePath);
	if (! in.is_open()) {
		in.close();
		throw std::logic_error("Error: unable to open file to decode it.");
	}

	std::wstring text;
	std::wstring line;
	in.imbue(std::locale(in.getloc(), new std::codecvt_utf8<wchar_t>));
	while (std::getline<wchar_t>(in, line, L'\n')) {
		text += line + L'\n';
	}
	in.close();
	return text;
}

std::string readEncodedFileDebugMode(const std::string& filePath) {
	std::ifstream fin(filePath);
	if (! fin.is_open()) {
		fin.close();
		throw std::runtime_error("Error: unable to open encoded file.");
	}
	std::string text;
	std::string line;
	while (std::getline(fin, line)) {
		text += line;
	}
	fin.close();
	return text;
}

std::string readEncodedFileBinaryMode(const std::string& filePath) {
	std::ifstream fin(filePath, std::ios::binary);
	if (! fin.is_open()) {
		fin.close();
		throw std::runtime_error("Error: unable to open encoded file.");
	}

	std::string result;
	char charSingleByte;
	while (fin.get(charSingleByte)) {
		std::bitset<8> bitsetSingleByte(charSingleByte);
		result += bitsetSingleByte.to_string();
	}
	fin.close();
	return result;
}

DecodingKey parseKeyFile(const std::string& filePath) {
	std::ifstream fis;
	fis.open(filePath);
	if (!fis.is_open()) {
		throw std::runtime_error("Error: unable to open key-file.");
	}

	DecodingKey decodingKey(fis);
	if (fis.bad()) {
		fis.close();
		throw std::runtime_error("Error: unable to read key-file.");
	}
	fis.close();
	return decodingKey;
}

void saveKeyFile(const EncodingKey& encodingKey) {
	std::ofstream fout;
	fout.open(KEY_FILE_NAME);
	if (!fout.is_open()) {
		fout.close();
		throw std::runtime_error("Error: unable to write key-file.");
	}
	fout << encodingKey;
	if (!fout) {
		fout.close();
		throw std::runtime_error("Error: unable to write key-file.");
	}
	fout.close();
}

void saveEncodedFileDebugMode(const std::string& encodedText) {
	std::ofstream out;
	out.open(ENCODED_DEBUG_MODE_FILE_NAME);
	if (!out.is_open()) {
		out.close();
		throw std::runtime_error("Error: unable to write encoded file.");
	}
	out << encodedText;
	out.close();
}

void saveEncodedFileBinaryMode(std::string& encodedText, const int& residualZeroes) {
	for (int i = 0; i < residualZeroes; ++i) {
		encodedText += "0";
	}

	std::ofstream out;
	out.open(ENCODED_FILE_NAME, std::ios::binary);
	if (!out.is_open()) {
		out.close();
		throw std::runtime_error("Error: unable to write encoded file.");
	}

	const size_t listBytesSize = (encodedText.size() + 7) / 8;
	ArrayList<unsigned char> listBytes(listBytesSize, 0);
	for (int i = 0; i < listBytes.size(); ++i) {
		if (encodedText[i] == '1') {
			listBytes.get(i / 8) |= (1 << (7 - (i % 8)));
		}
	}

	out.write(reinterpret_cast<const char*>(listBytes.data()), listBytes.size());
	out.close();
}

void saveDecodedFile(const std::wstring& text) {
	std::wofstream out;
	out.open(DECODED_FILE_NAME, std::ios::out);
	out.imbue(std::locale(out.getloc(), new std::codecvt_utf8<wchar_t>));
	if (!out.is_open()) {
		out.close();
		throw std::logic_error("Error: unable to save decoded file.");
	}
	out << text;
	out.close();
}
