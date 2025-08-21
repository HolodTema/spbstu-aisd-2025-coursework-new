#ifndef FILE_HELPER_H
#define FILE_HELPER_H
#include <codecvt>
#include <fstream>
#include <bits/stdc++.h>
#include "config.h"
#include "EncryptionKey.h"
#include "exceptions.h"

class EncryptionKey;

class FileHelper {
public:

    static std::pair<std::wstring, bool> readTextFile(const std::string& filePath) {
        std::wifstream in(filePath);
        if (! in.is_open()) {
            in.close();
            return std::make_pair(std::wstring(), false);
        }
        std::wstring text;
        std::wstring line;
        in.imbue(std::locale(in.getloc(), new std::codecvt_utf8<wchar_t>));
        while (std::getline<wchar_t>(in, line, L'\n')) {
            text += line + L'\n';
        }
        in.close();
        return std::make_pair(text, true);
    }

    static std::string readEncodedTextFileDebugMode(const std::string& filePath) {
        std::ifstream in(filePath);
        if (! in.is_open()) {
            in.close();
            throw UnableToOpenEncodedTextFile();
        }
        std::string text;
        std::string line;
        while (std::getline(in, line)) {
            text += line;
        }
        in.close();
        return text;
    }

    static bool saveKeyFile(const std::string& textKeyFile) {
        std::ofstream out;
        out.open(KEY_FILE_NAME);
        if (!out.is_open()) {
            out.close();
            return false;
        }
        out << textKeyFile;
        out.close();
        return true;
    }

    static bool saveEncodedTextDebugMode(const std::string& encodedText) {
        std::ofstream out;
        out.open(ENCODED_DEBUG_MODE_FILE_NAME);
        if (!out.is_open()) {
            out.close();
            return false;
        }
        out << encodedText;
        out.close();
        return true;
    }

    static bool saveEncodedText(std::string& encodedText, const int& residualZeroes) {
        for (int i = 0; i < residualZeroes; ++i) {
            encodedText += "0";
        }
        std::bitset<8> bitsetToSave(encodedText);

        std::ofstream out;
        out.open(ENCODED_FILE_NAME, std::ios::binary);
        if (!out.is_open()) {
            out.close();
            return false;
        }

        unsigned long n = bitsetToSave.to_ulong();
        out.write(reinterpret_cast<char*>(&n), sizeof(n));
        out.close();
        return true;
    }

    static bool saveDecodedText(const std::wstring& text) {
        std::wofstream out;
        out.open(DECODED_FILE_NAME, std::ios::out);
        out.imbue(std::locale(out.getloc(), new std::codecvt_utf8<wchar_t>));
        if (!out.is_open()) {
            out.close();
            return false;
        }
        out << text;
        out.close();
        return true;
    }

    static EncryptionKey parseKeyFile() {
        std::ifstream in;
        in.open(KEY_FILE_NAME);
        if (!in.is_open()) {
            throw UnableToParseKeyFileException();
        }

        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int residualZeroes = 0;
        in >> residualZeroes;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::unordered_map<std::string, wchar_t> mapCodes;
        std::string line;
        const char* delim = " ";
        while (std::getline(in, line)) {
            unsigned int charOrd = std::stoi(line.substr(0, line.find(delim)));
            std::string charCode = line.substr(line.find(delim) + 1);
            mapCodes[charCode] = static_cast<wchar_t>(charOrd);
        }
        in.close();
        return EncryptionKey(mapCodes, residualZeroes);
    }

};

#endif //FILEHELPER_H
