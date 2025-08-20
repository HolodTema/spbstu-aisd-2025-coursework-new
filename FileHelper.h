#ifndef FILE_HELPER_H
#define FILE_HELPER_H
#include <codecvt>
#include <fstream>
#include "config.h"

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

};

#endif //FILEHELPER_H
