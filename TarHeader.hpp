//#ifdef TAR_HEADER_HPP
//#define TAR_HEADER_HPP
#define LINUX

#include <algorithm>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <array>
#include <stdint.h>

#include <iomanip>

#include "sha256.h"

#define VERSION "01"

struct TarHeader {
    enum class TYPELAGS : char {
        REGTYPE = '0',
        AREGTYPE = '\0',
        LNKTYPE = '1',
        SYMTYPE = '2',
        CHRTYPE = '3',
        BLKTYPE = '4',
        DIRTYPE = '5',
        FIFOTYPE = '6',
        CONTTYPE = '7',
        XHDTYPE = 'x',
        XGLTYPE = 'g',
        LONGNAMETYPE = 'L',
        LONGLINKTYPE = 'K',
    };

    std::array<char, 100> fileName = {};
    std::array<char, 10> formatTar = {}; //name tar
    std::array<char, 8> mode = {}; //mode
    std::array<char, 8> uid = {};
    std::array<char, 8> gid = {};
    std::array<char, 12> mtime = {};
    std::array<char, 65> chksum = {};
    TYPELAGS typeFlag = {};
    std::array<char, 32> uname = {};
    std::array<char, 32> gname = {};
    std::array<char, 8> devmajor = {};
    std::array<char, 8> devminor = {};
    std::array<char, 12> atime = {}; //Время последнего доступа к файлу
    std::array<char, 12> ctime = {}; //Время последего изм прав дос
    std::array<char, 2> version = {};
    std::array<char, 194> prefix = {};


    template<size_t N>
    static void decToHexStr(std::array<char, N>& octalStr, uintmax_t decNumber, uint8_t flag_b = 2) noexcept;
    
    static TYPELAGS getTypeFlag(const mode_t mode) noexcept;

    void calcChecksum(const char* data) noexcept; // Данную функцию следует вызывать в самом конце
};

template<size_t N>
inline void TarHeader::decToHexStr(std::array<char, N>& hexStr, uintmax_t decNumber, uint8_t flag_b) noexcept{
    // Создаем строковый поток
    std::ostringstream stream;

    // В зависимости от значения flag_b, выбираем формат вывода (с префиксом 0x или без)
    // if (flag_b {
    //     stream << "0x";
    // }

    // Устанавливаем ширину поля и заполняем нулями
    stream << std::setfill('0') << std::setw(N - flag_b * 2) << std::hex << decNumber;

    // Копируем результат в массив hexStr
    std::string result = stream.str();
    std::copy(result.begin(), result.end(), hexStr.begin());

    // Заполняем оставшиеся байты нулями, если строка получилась короче
    if (result.size() < N) {
        std::fill(hexStr.begin() + result.size(), hexStr.end(), '\0');
    }
}

inline void TarHeader::calcChecksum(const char* data) noexcept {
    chksum.fill('\0'); // Заполняем именно \0!!!
    uint64_t sum = 0;
    for(size_t i = 0; i < sizeof(TarHeader); ++i)
        sum += reinterpret_cast<unsigned char*>(this)[i];
    std::array<char, 8> sumFlags = {};
    decToHexStr(sumFlags, sum, 0);
    std::string hashA;
    if(data != nullptr)
        hashA = std::string(data);
    
    for(auto it : sumFlags)
        hashA.push_back(it);

    std::string hash = sha256(hashA);
    for(int i = 0; i < hash.size(); i++)
        chksum[i] = hash[i];
}


inline TarHeader::TYPELAGS TarHeader::getTypeFlag(const mode_t mode) noexcept {
    if(S_ISREG(mode)) return TarHeader::TYPELAGS::REGTYPE;
    if(S_ISDIR(mode)) return TarHeader::TYPELAGS::DIRTYPE;
    if(S_ISLNK(mode)) return TarHeader::TYPELAGS::LNKTYPE;
                    return TarHeader::TYPELAGS::AREGTYPE;
}

inline uint64_t expandSizeTo512Blocks(uint64_t size) {
    return (sizeof(TarHeader) - size % sizeof(TarHeader)) % sizeof(TarHeader);
}
