#ifndef CODE_HELPER_H
#define CODE_HELPER_H

#include <string>
#include <limits>
#include "config.h"
#include "FileHelper.h"
#include "HashMap.hpp"

class CodeHelper {
public:
    static bool encodeText(const std::wstring& textToEncode, bool isDebugMode) {
        HashMap<wchar_t, int> mapCharsWithAmount(textToEncode.size());
        for (wchar_t ch : textToEncode) {
            if (mapCharsWithAmount.contains(ch)) {
                mapCharsWithAmount.insert(ch, mapCharsWithAmount.get(ch)+1);
            }
            else {
                mapCharsWithAmount.insert(ch, 1);
            }
        }

        ArrayList<CharInfo> listCharInfo(mapCharsWithAmount.amountElements());

        auto iterationCallback = [&listCharInfo](HashMap<wchar_t, int>::Pair& pair) {
            listCharInfo.add(CharInfo(pair.key, pair.value, std::string()));
            return true;
        };
        mapCharsWithAmount.forEach(iterationCallback);
        listCharInfo.sort(CharInfo::compare);
        handleEncoding(listCharInfo, 0, listCharInfo.size(), textToEncode.size());
        HashMap<wchar_t, std::string> mapCodes = convertCharInfoVectorToCodesMap(listCharInfo);
        std::string encodedText = generateEncodedTextDebugMode(textToEncode, mapCodes);

        std::string textKeyFile = generateKeyFileText(listCharInfo, getResidualZeroes(encodedText));
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
        HashMap<std::string, wchar_t> mapCodes = encryptionKey.getMapCodes();
        for (int i = 0; i < encodedText.size()-encryptionKey.getResidualZeroes(); ++i) {
            currentCode += encodedText[i];
            if (mapCodes.contains(currentCode)) {
                result += mapCodes.get(currentCode);
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

    static void handleEncoding(ArrayList<CharInfo>& listCharInfo, const size_t& low, const size_t& high, const int& textLen) {
        if (low == high) {
            return;
        }

        int firstPart = 0;
        int secondPart = textLen;

        int targetFirstPartLen = 0;
        int targetSecondPartLen = 0;

        int minDifference = std::numeric_limits<int>::max();
        auto borderCounter = low;

        for (size_t currentCounter = low; currentCounter < high; ++currentCounter) {
            CharInfo charInfo = listCharInfo.get(currentCounter);
            firstPart += charInfo.amount;
            secondPart -= charInfo.amount;
            if (std::abs(firstPart - secondPart) < minDifference) {
                minDifference = std::abs(firstPart - secondPart);
                borderCounter = currentCounter;
                targetFirstPartLen = firstPart;
                targetSecondPartLen = secondPart;
            }
        }

        borderCounter++;
        if (borderCounter == high) {
            return;
        }

        addZeroToCode(listCharInfo, low, borderCounter);
        addOneToCode(listCharInfo, borderCounter, high);

        handleEncoding(listCharInfo, low, borderCounter, targetFirstPartLen);
        handleEncoding(listCharInfo, borderCounter, high, targetSecondPartLen);
    }

    static void addZeroToCode(ArrayList<CharInfo>& listCharInfo, size_t low, size_t high) {
        while (low != high) {
            listCharInfo.get(low).code += '0';
            ++low;
        }
    }

    static void addOneToCode(ArrayList<CharInfo>& listCharInfo, size_t low, size_t high) {
        while (low != high) {
            listCharInfo.get(low).code += '1';
            ++low;
        }
    }

    static std::string generateKeyFileText(const ArrayList<CharInfo>& listCharInfo, int residualZeroes) {
        std::string result = "Key-file for Shannon-Fano encoding. Version ";
        result += PROGRAM_VERSION + "\n";
        result += std::to_string(residualZeroes) + "\n";
        for (int i = 0; i < listCharInfo.size(); ++i) {
            CharInfo charInfo = listCharInfo.get(i);
            unsigned int ord = static_cast<unsigned int>(charInfo.ch);
            result += std::to_string(ord) + " " + charInfo.code + "\n";
        }
        return result;
    }

    static HashMap<wchar_t, std::string> convertCharInfoVectorToCodesMap(const ArrayList<CharInfo>& listCharInfo) {
        HashMap<wchar_t, std::string> map(listCharInfo.size());
        for (int i = 0; i < listCharInfo.size(); ++i) {
            CharInfo charInfo = listCharInfo.get(i);
            map.insert(charInfo.ch, charInfo.code);
        }
        return map;
    }

    static std::string generateEncodedTextDebugMode(const std::wstring& textToEncode, const HashMap<wchar_t, std::string>& mapCodes) {
        std::string result;
        for (wchar_t ch : textToEncode) {
            result += mapCodes.get(ch);
        }
        return result;
    }

    static int getResidualZeroes(const std::string& encodedText) {
        return static_cast<int>(encodedText.size() % 8);
    }
};

#endif //CODE_HELPER_H
