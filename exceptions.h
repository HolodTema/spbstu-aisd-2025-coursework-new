#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class UnableToParseKeyFileException final: public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: unable to parse key-file.\n";
    }
};

class UnableToOpenEncodedTextFile final: public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: unable to open file with encoded text to decode it.\n";
    }
};

#endif //EXCEPTIONS_H
