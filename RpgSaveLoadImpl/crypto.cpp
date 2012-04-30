#include <windows.h>
#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "crypto.hpp"
#include "debug.hpp"
#include "util.hpp"

using namespace std;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;
using CryptoPP::Exception;
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using namespace RpgSaveLoadImpl;

unsigned char Crypto::m_key[] = { 0x51, 0x76, 0x4a, 0x33, 0x64, 0x64, 0x31, 0x4a, 0x33, 0x64, 0x64, 0x31, 0x2d, 0x2f, 0x2e, 0x3f };
unsigned char Crypto::m_iv[] = { 0x43, 0x72, 0x34, 0x7a, 0x79, 0x46, 0x61, 0x3f, 0x61, 0x41, 0x41, 0x41, 0x61, 0x61, 0x6b, 0x6b };

const string Crypto::Encrypt(const string &plainText)
{
   string cipher, encoded;

   try {
        CBC_Mode<AES>::Encryption enc;
        enc.SetKeyWithIV(m_key, sizeof(m_key), m_iv, sizeof(m_iv));

        cipher.clear();
        StringSource(plainText, true, new StreamTransformationFilter(enc, new StringSink(cipher)));

        encoded.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
    }
    catch(CryptoPP::Exception &ex) {
		LOG("Encryption", ex.what(), plainText);
    }
    catch (...) {
		LOG("Unknown Encryption Error!!", plainText);
    }

    return encoded;
}

const string Crypto::Decrypt(const string &cipherText)
{
    string cipher, recovered;

    try {
        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(m_key, sizeof(m_key), m_iv, sizeof(m_iv));

        cipher.clear();
        StringSource(cipherText, true, new HexDecoder(new StringSink(cipher)));

        recovered.clear();
        StringSource s(cipher, true, new StreamTransformationFilter(dec, new StringSink(recovered)));
    }
    catch(CryptoPP::Exception &ex) {
		LOG("Decryption", ex.what(), cipherText);
    }
    catch (...) {
		LOG("Unknown Decryption Error!!", cipherText);
    }

    return recovered;
}

