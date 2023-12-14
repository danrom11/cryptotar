#include <unistd.h>

#define VERSION_CRYPTO_MODULE "01"

class cryptoModule {
public:
    virtual ~cryptoModule() = default;
    virtual void* cryptoData(unsigned char*, size_t) = 0;
    virtual void* uncryptoData(unsigned char*, size_t) = 0;


    void setKey(char* key, size_t sizeKey){
        this->key = key;
        this->sizeKey = sizeKey;
    }

    char* key = NULL;
    size_t sizeKey = 0;
};
