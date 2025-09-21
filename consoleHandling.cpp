#include "consoleHandling.hpp"
#include <iostream>
#include "config.h"
#include "fileHandling.hpp"
#include "encodingHandling.hpp"

namespace {
	void onEncodeOption(const bool isDebugMode) {
        std::cout << "\nEnter path to text file to encode it. (The text file must be encoded in utf-8):\n";
        std::string filePath;
        std::cin >> filePath;
        if (!std::cin) {
            throw std::runtime_error("Error: invalid file path.");
        }

        auto textToEncode  = readFileToEncode(filePath);
	    encodeText(textToEncode, isDebugMode);

        std::cout << "Success!\n";
        if (isDebugMode) {
            std::cout << "Encoded text was saved in debug mode in file: " + ENCODED_DEBUG_MODE_FILE_NAME + "\n";
        }
        else {
            std::cout << "Encoded text was saved in file: " + ENCODED_FILE_NAME + "\n";
        }
        std::cout << "Key-file was saved: " + KEY_FILE_NAME + "\n";
    }

    void onDecodeOption(const bool isDebugMode) {
        if (isDebugMode) {
            std::cout << "Enter path to the file with encoded text in debug mode:\n";
        }
        else {
            std::cout << "Enter path to the binary file with encoded text:\n";
        }
        std::string filePathEncodedText;
        std::cin >> filePathEncodedText;
        if (!std::cin) {
            throw std::runtime_error("Error: invalid file path.");
        }

        std::string encodedText;
        if (isDebugMode) {
            encodedText = readEncodedFileDebugMode(filePathEncodedText);
        }
        else {
            encodedText = readEncodedFileBinaryMode(filePathEncodedText);
        }

        std::cout << "Enter path to key-file:\n";
        std::string keyFilePath;
        std::cin >> keyFilePath;
        if (!std::cin) {
            throw std::runtime_error("Error: invalid file path.");
        }

        DecodingKey decodingKey = parseKeyFile(keyFilePath);
	    decodeTextDebugMode(encodedText, decodingKey);
	    std::cout << "Success!\n";
	    std::cout << "Decoded text was saved in file " + DECODED_FILE_NAME + "\n";

    }

}

void launch() {
    std::cout << "Hello, this program allows you to encode or decode text using Shannon-Fano coding.\n\n";
    std::cout << "What do you want to do?\n";
    std::cout << "1. Encode text file\n";
    std::cout << "2. Encode text file in debug mode\n";
    std::cout << "3. Decode text file\n";
    std::cout << "4. Decode text file in debug mode\n";

    int option = 0;
    std::cin >> option;
    if (!std::cin) {
        throw std::runtime_error("Error: invalid option.");
    }

    if (option == 1) {
        onEncodeOption(false);
    }
    else if (option == 2) {
        onEncodeOption(true);
    }
    else if (option == 3) {
        onDecodeOption(false);
    }
    else if (option == 4) {
        onDecodeOption(true);
    }
    else {
        throw std::runtime_error("Error: invalid option.");
    }
}