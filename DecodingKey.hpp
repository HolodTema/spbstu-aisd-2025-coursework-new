#ifndef DECODING_KEY_HPP
#define DECODING_KEY_HPP

#include <string>
#include "HashMap.hpp"

struct DecodingKey {
    explicit DecodingKey(std::istream& is);

    HashMap<std::string, wchar_t> getMapCodes() const;

    int getResidualZeroes() const;

private:
    HashMap<std::string, wchar_t> mapCodes_;
    int residualZeroes_ = 0;
};

#endif