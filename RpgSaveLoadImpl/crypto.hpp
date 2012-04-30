#ifndef CRYPTO_HPP
#define CRYPTO_HPP


#include <string>

namespace RpgSaveLoadImpl {
    class Crypto;
}

class RpgSaveLoadImpl::Crypto
{
private:
    static unsigned char m_key[];
    static unsigned char m_iv[];

public:
    static const std::string Encrypt(const std::string &plainText);
    static const std::string Decrypt(const std::string &cipherText);
};


#endif /* CRYPTO_HPP */


