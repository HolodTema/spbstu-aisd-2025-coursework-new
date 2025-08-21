#ifndef СONSOLE_HELPER_H
#define CONSOLE_HELPER_H
#include <iostream>

#include "CodeHelper.h"
#include "FileHelper.h"

class ConsoleHelper {
public:

    static int launch() {
        std::cout << "Hello, this program allows you to encode or decode text using Shannon-Fano coding.\n\n";
        std::cout << "What do you want to do?\n";
        std::cout << "1. Encode text file\n";
        std::cout << "2. Encode text file in debug mode\n";
        std::cout << "3. Decode text file\n";
        std::cout << "4. Decode text file in debug mode\n";

        int option = 0;
        std::cin >> option;
        if (!std::cin) {
            return onErrorInvalidOption();
        }

        if (option == 1) {
            return onEncodeOption(false);
        }
        if (option == 2) {
            return onEncodeOption(true);
        }
        if (option == 3) {
            return onDecodeOption(false);
        }
        if (option == 4) {
            return onDecodeOption(true);
        }
        return onErrorInvalidOption();
    }

private:

    static int onEncodeOption(bool isDebugMode) {
        std::cout << "\nEnter path to text file to encode it. (The text file must be encoded in utf-8):\n";
        std::string filePath;
        std::cin >> filePath;
        if (!std::cin) {
            return onErrorInvalidFilePath();
        }

        auto pairTextToEncode = FileHelper::readTextFile(filePath);
        if (!pairTextToEncode.second) {
            return onErrorInvalidFilePath();
        }
        std::wstring textToEncode = pairTextToEncode.first;

        bool isSuccessful = CodeHelper::encodeText(textToEncode, isDebugMode);
        if (!isSuccessful) {
            return onErrorUnableToEncode();
        }

        std::cout << "Success!\n";
        if (isDebugMode) {
            std::cout << "Encoded text was saved in debug mode in file: " + ENCODED_DEBUG_MODE_FILE_NAME + "\n";
        }
        else {
            std::cout << "Encoded text was saved in file: " + ENCODED_FILE_NAME + "\n";
        }
        std::cout << "Key-file was saved: " + KEY_FILE_NAME + "\n";
        return 0;
    }

    static int onDecodeOption(bool isDebugMode) {
        try {
            if (isDebugMode) {
                std::cout << "Enter path to the file with encoded text in debug mode:\n";
            }
            else {
                std::cout << "Enter path to the binary file with encoded text:\n";
            }
            std::string filePath;
            std::cin >> filePath;
            if (!std::cin) {
                return onErrorInvalidFilePath();
            }

            std::string encodedText;
            if (isDebugMode) {
                encodedText = FileHelper::readEncodedTextFileDebugMode(filePath);
            }
            else {
                encodedText = FileHelper::readEncodedTextFile(filePath);
            }

            EncryptionKey encryptionKey = FileHelper::parseKeyFile();

            bool isSuccessful = CodeHelper::decodeTextDebugMode(encodedText, encryptionKey);

            if (isSuccessful) {
                std::cout << "Success!\n";
                std::cout << "Decoded text was saved in file " + DECODED_FILE_NAME + "\n";
                return 0;
            }
            else {
                return onErrorUnableToDecode();
            }
        }
        catch (const UnableToParseKeyFileException& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
        catch (const UnableToOpenEncodedTextFile& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }


    }

    static int onErrorInvalidOption() {
        std::cout << "Error: invalid option.\n";
        return 1;
    }

    static int onErrorInvalidFilePath() {
        std::cout << "Error: invalid file path.\n";
        return 1;
    }

    static int onErrorUnableToEncode() {
        std::cout << "Error: unable to encode this text.\n";
        return 1;
    }

    static int onErrorUnableToDecode() {
        std::cout << "Error: unable to decode this encoded text.\n";
        return 1;
    }
};

#endif //CONSOLE_HELPER_H
