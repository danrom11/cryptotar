#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rc4CryptoModule.hpp"

#define N 256   // 2^8




class crypto : public cryptoModule{
public:
    void* cryptoData(unsigned char* data, size_t size) override {
        unsigned char* cipherText = new unsigned char[size];
        
        unsigned char S[N];

        initialize(S, (unsigned char*)this->key, this->sizeKey);

        RC4(S, data, cipherText, size);

        return cipherText;
    }

    void* uncryptoData(unsigned char* data, size_t size) override{
        unsigned char* cipherText = new unsigned char[size];
        
        unsigned char S[N];

        initialize(S, (unsigned char*)this->key, this->sizeKey);

        RC4(S, data, cipherText, size);

        return cipherText;
    }

private:
    void initialize(unsigned char S[], const unsigned char key[], int key_length) {
        for (int i = 0; i < N; i++) {
            S[i] = i;
        }

        unsigned char j = 0;
        for (int i = 0; i < N; i++) {
            j = (j + S[i] + key[i % key_length]) % N;
            unsigned char temp = S[i];
            S[i] = S[j];
            S[j] = temp;
        }
    }

    // Функция генерации псевдослучайной последовательности
    void RC4(unsigned char S[], const unsigned char *plaintext, unsigned char *ciphertext, int length) {
        unsigned char i = 0;
        unsigned char j = 0;

        for (size_t k = 0; k < length; k++) {
            i = (i + 1) % N;
            j = (j + S[i]) % N;

            unsigned char temp = S[i];
            S[i] = S[j];
            S[j] = temp;

            unsigned char rnd = S[(S[i] + S[j]) % N];
            ciphertext[k] = rnd ^ plaintext[k];
        }
    }
};


cryptoModule* makechild() {
    return new crypto();
}






