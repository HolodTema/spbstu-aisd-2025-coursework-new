#include "encodingHandling.hpp"
#include <limits>

#include "config.h"
#include "fileHandling.hpp"

namespace {
	void addZeroToCode(ArrayList<CharInfo>& listCharInfo, size_t low, size_t high) {
		while (low != high) {
			listCharInfo.get(low).code += '0';
			++low;
		}
	}

	void addOneToCode(ArrayList<CharInfo>& listCharInfo, size_t low, size_t high) {
		while (low != high) {
			listCharInfo.get(low).code += '1';
			++low;
		}
	}

	 void handleEncoding(ArrayList<CharInfo>& listCharInfo, const size_t& low, const size_t& high, const int& textLen) {
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

	HashMap<wchar_t, std::string> convertCharInfoListToMapCodes(const ArrayList<CharInfo>& listCharInfo) {
		HashMap<wchar_t, std::string> map(listCharInfo.size());
		for (int i = 0; i < listCharInfo.size(); ++i) {
		    CharInfo charInfo = listCharInfo.get(i);
		    map.insert(charInfo.ch, charInfo.code);
		}
		return map;
	}

	std::string generateEncodedTextDebugMode(const std::wstring& textToEncode, const HashMap<wchar_t, std::string>& mapCodes) {
		std::string result;
		for (wchar_t ch : textToEncode) {
		    result += mapCodes.get(ch);
		}
		return result;
	}

	int getResidualZeroes(const std::string& encodedText) {
		return static_cast<int>(encodedText.size() % 8);
	}
}

void encodeText(const std::wstring& textToEncode, bool isDebugMode) {
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
	HashMap<wchar_t, std::string> mapCodes = convertCharInfoListToMapCodes(listCharInfo);
	std::string encodedText = generateEncodedTextDebugMode(textToEncode, mapCodes);

	EncodingKey encodingKey(mapCodes, getResidualZeroes(encodedText));
	saveKeyFile(encodingKey);

	if (isDebugMode) {
		saveEncodedFileDebugMode(encodedText);
	}
	else {
		saveEncodedFileBinaryMode(encodedText, getResidualZeroes(encodedText));
	}
}

void decodeTextDebugMode(const std::string& encodedText, const DecodingKey& decodingKey) {
	std::wstring result;
	std::string currentCode;
	HashMap<std::string, wchar_t> mapCodes = decodingKey.getMapCodes();
	for (int i = 0; i < encodedText.size()-decodingKey.getResidualZeroes(); ++i) {
		currentCode += encodedText[i];
		if (mapCodes.contains(currentCode)) {
			result += mapCodes.get(currentCode);
			currentCode.clear();
		}
	}
	saveDecodedFile(result);
}