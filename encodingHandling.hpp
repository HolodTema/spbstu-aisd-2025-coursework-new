#ifndef ENCODING_HANDLING_HPP
#define ENCODING_HANDLING_HPP

#include <string>
#include "fileHandling.hpp"

struct CharInfo {
    wchar_t ch;
    int amount;
    std::string code;

    static int compare(const CharInfo& lhs, const CharInfo& rhs) {
        if (lhs.amount > rhs.amount) {
            return -1;
        }
        if (lhs.amount == rhs.amount) {
            return 0;
        }
        return 1;
    }
};

void encodeText(const std::wstring& textToEncode, bool isDebugMode);

void decodeTextDebugMode(const std::string& encodedText, const DecodingKey& decodingKey);

#endif
