#ifndef ENCRYPTIONKEY_H
#define ENCRYPTIONKEY_H
#include <string>
#include <unordered_map>

class EncryptionKey {
public:

    EncryptionKey() = default;

    EncryptionKey(std::unordered_map<std::string, wchar_t> mapCodes, int residualZeroes):
        mapCodes(mapCodes),
        residualZeroes(residualZeroes)
    { }

    std::unordered_map<std::string, wchar_t> getMapCodes() const {
        return mapCodes;
    }

    int getResidualZeroes() const {
        return residualZeroes;
    }

private:
    std::unordered_map<std::string, wchar_t> mapCodes;
    int residualZeroes = 0;

};

#endif //ENCRYPTIONKEY_H
