#ifndef CODE_HELPER_H
#define CODE_HELPER_H

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include "config.h"
#include "FileHelper.h"

class CodeHelper {
public:

    static bool encodeText(const std::wstring& textToEncode, bool isDebugMode) {
        std::unordered_map<wchar_t, int> mapCharsWithAmount;
        for (wchar_t ch : textToEncode) {
            if (mapCharsWithAmount.contains(ch)) {
                mapCharsWithAmount[ch] += 1;
            }
            else {
                mapCharsWithAmount[ch] = 1;
            }
        }

        std::vector<CharInfo> vectorCharInfo;
        for (std::pair<wchar_t, int> pair : mapCharsWithAmount) {
            vectorCharInfo.push_back(CharInfo(pair.first, pair.second, std::string()));
        }

        std::sort(vectorCharInfo.begin(), vectorCharInfo.end(), CharInfo::compare);

        handleEncoding(vectorCharInfo.begin(), vectorCharInfo.end(), textToEncode.size());

        std::unordered_map<wchar_t, std::string> mapCodes = convertCharInfoVectorToCodesMap(vectorCharInfo);
        std::string encodedText = generateEncodedTextDebugMode(textToEncode, mapCodes);

        std::string textKeyFile = generateKeyFileText(vectorCharInfo, getResidualZeroes(encodedText));
        if (!FileHelper::saveKeyFile(textKeyFile)) {
            return false;
        }

        if (isDebugMode) {
            if (!FileHelper::saveEncodedTextDebugMode(encodedText)) {
                return false;
            }
        }
        else {
            if (!FileHelper::saveEncodedText(encodedText, getResidualZeroes(encodedText))) {
                return false;
            }
        }
        return true;
    }

    static bool decodeTextDebugMode(const std::string& encodedText, const EncryptionKey& encryptionKey) {
        std::wstring result;
        std::string currentCode;
        std::unordered_map<std::string, wchar_t> mapCodes = encryptionKey.getMapCodes();
        for (int i = 0; i < encodedText.size()-encryptionKey.getResidualZeroes(); ++i) {
            currentCode += encodedText[i];
            if (mapCodes.contains(currentCode)) {
                result += mapCodes[currentCode];
                currentCode.clear();
            }
        }

        return FileHelper::saveDecodedText(result);
    }

private:

    struct CharInfo {
        wchar_t ch;
        int amount;
        std::string code;

        static bool compare(const CharInfo& lhs, const CharInfo& rhs) {
            return lhs.amount > rhs.amount;
        }
    };

    static void handleEncoding(std::vector<CharInfo>::iterator beginIter, std::vector<CharInfo>::iterator endIter, const int& textLen) {
        if (beginIter == endIter) {
            return;
        }

        int firstPart = 0;
        int secondPart = textLen;

        int targetFirstPartLen = 0;
        int targetSecondPartLen = 0;

        int minDifference = std::numeric_limits<int>::max();
        auto borderIter = beginIter;

        auto currentIter = beginIter;
        while (currentIter != endIter) {
            firstPart += currentIter->amount;
            secondPart -= currentIter->amount;
            if (std::abs(firstPart - secondPart) < minDifference) {
                minDifference = std::abs(firstPart - secondPart);
                borderIter = currentIter;
                targetFirstPartLen = firstPart;
                targetSecondPartLen = secondPart;
            }
            ++currentIter;
        }

        borderIter++;
        if (borderIter == endIter) {
            return;
        }

        addZeroToCode(beginIter, borderIter);
        addOneToCode(borderIter, endIter);

        handleEncoding(beginIter, borderIter, targetFirstPartLen);
        handleEncoding(borderIter, endIter, targetSecondPartLen);
    }

    static void addZeroToCode(std::vector<CharInfo>::iterator beginIter, std::vector<CharInfo>::iterator endIter) {
        while (beginIter != endIter) {
            beginIter->code += '0';
            ++beginIter;
        }
    }

    static void addOneToCode(std::vector<CharInfo>::iterator beginIter, std::vector<CharInfo>::iterator endIter) {
        while (beginIter != endIter) {
            beginIter->code += '1';
            ++beginIter;
        }
    }

    static std::string generateKeyFileText(const std::vector<CharInfo>& vectorCharInfo, int residualZeroes) {
        std::string result = "Key-file for Shannon-Fano encoding. Version ";
        result += PROGRAM_VERSION + "\n";
        result += std::to_string(residualZeroes) + "\n";
        for (CharInfo charInfo : vectorCharInfo) {
            unsigned int ord = static_cast<unsigned int>(charInfo.ch);
            result += std::to_string(ord) + " " + charInfo.code + "\n";
        }
        return result;
    }

    static std::unordered_map<wchar_t, std::string> convertCharInfoVectorToCodesMap(const std::vector<CharInfo>& vectorCharInfo) {
        std::unordered_map<wchar_t, std::string> map;
        for (const CharInfo& charInfo : vectorCharInfo) {
            map[charInfo.ch] = charInfo.code;
        }
        return map;
    }

    static std::string generateEncodedTextDebugMode(const std::wstring& textToEncode, const std::unordered_map<wchar_t, std::string>& mapCodes) {
        std::string result;
        for (wchar_t ch : textToEncode) {
            result += mapCodes.at(ch);
        }
        return result;
    }

    static int getResidualZeroes(const std::string& encodedText) {
        return static_cast<int>(encodedText.size() % 8);
    }
};

#endif //CODE_HELPER_H
