#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

class UnableToParseKeyFileException: public std::exception {
public:

    const char* what() const noexcept override {
        return "Error: unable to parse key-file.\n";
    }

};

class UnableToOpenEncodedTextFile: public std::exception {
public:

    const char* what() const noexcept override {
        return "Error: unable to open file with encoded text to decode it.\n";
    }

};
#endif //EXCEPTIONS_H
