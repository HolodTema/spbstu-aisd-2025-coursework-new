#ifndef ENCODING_KEY_HPP
#define ENCODING_KEY_HPP

#include <string>
#include "HashMap.hpp"

struct EncodingKey {
    explicit EncodingKey(const HashMap<wchar_t, std::string>& mapCodes, const int& residualZeroes):
        mapCodes_(mapCodes),
        residualZeroes_(residualZeroes)
    { }

    HashMap<wchar_t, std::string> getMapCodes() const;

    int getResidualZeroes() const;

    friend std::ostream& operator<<(std::ostream& os, const EncodingKey& encodingKey);

private:
    HashMap<wchar_t, std::string> mapCodes_;
    int residualZeroes_ = 0;
};

std::ostream& operator<<(std::ostream& os, const EncodingKey& encodingKey);

#endif
