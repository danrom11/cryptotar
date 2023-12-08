#include <iostream>
#include "xorCryptoModule.hpp"

class crypto : public cryptoModule{
public:
    void* cryptoData(unsigned char* data, size_t size) override {
        for(size_t i = 0; i < size; i++){
            data[i] ^= key[i % sizeKey];
        }
        return data;
    }

    void* uncryptoData(unsigned char* data, size_t size) override{
        for(size_t i = 0; i < size; i++){
            data[i] ^= key[i % sizeKey];
        }
        return data;
    }

};


cryptoModule* makechild() {
    return new crypto();
}

