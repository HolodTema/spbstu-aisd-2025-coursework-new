#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class UnableToParseKeyFileException final: public std::exception {
public:
    const char* what() const noexcept override;
};

class UnableToOpenEncodedTextFileException final: public std::exception {
public:
    const char* what() const noexcept override;
};

#endif