#ifndef ENCRYPTION_KEY_H
#define ENCRYPTION_KEY_H

#include <string>
#include "HashMap.hpp"

class EncryptionKey {
public:

    EncryptionKey() = default;

    EncryptionKey(HashMap<std::string, wchar_t>& mapCodes, int residualZeroes):
        mapCodes(mapCodes),
        residualZeroes(residualZeroes)
    { }

    EncryptionKey(const EncryptionKey& rhs) = delete;

    EncryptionKey& operator=(const EncryptionKey& rhs) = delete;

    EncryptionKey(EncryptionKey&& rhs) = delete;

    HashMap<std::string, wchar_t> getMapCodes() const {
        return mapCodes;
    }

    int getResidualZeroes() const {
        return residualZeroes;
    }

private:

    HashMap<std::string, wchar_t> mapCodes;
    int residualZeroes = 0;
};

#endif //ENCRYPTION_KEY_H
