#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <memory>

#ifdef LINUX
    #include <sys/sysmacros.h>
#endif

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"


#ifdef DEBUG
    #define DEBUG_PRINT_SEC(fmt, ...) fprintf(stderr, ANSI_COLOR_GREEN fmt ANSI_COLOR_RESET, __VA_ARGS__)
    #define DEBUG_PRINT_ERR(fmt, ...) fprintf(stderr, ANSI_COLOR_RED fmt ANSI_COLOR_RESET, __VA_ARGS__)
#else
    #define DEBUG_PRINT_SEC(fmt, ...)
    #define DEBUG_PRINT_ERR(fmt, ...)
#endif

#include "TarHeader.hpp"

using ProgressCallback = std::function<void(size_t bytesRead, size_t fileSize, char* fileName)>;
using TarHeaderCallback = std::function<void(TarHeader header, size_t size, char* path)>;

class cryptotar{
public:
    cryptotar() = default;
    cryptotar(std::string archiveName);
    cryptotar(std::string archiveName, std::vector<std::string>& paths);

    cryptotar(std::string pathToArhive, std::string ExtractToPath);

    ~cryptotar();

    ProgressCallback globalProgressCallback = [](size_t bytesRead, size_t fileSize, char* fileName){
        #ifdef DEBUG
            double progress = static_cast<double>(bytesRead) / fileSize * 100.0;
            std::cout << "File: " << fileName << " read: " << bytesRead << " out of " << fileSize << " bytes (" << progress << "%)" << std::endl;
        #endif
    };

    TarHeaderCallback globalTarHeaderCallback = [](TarHeader header, size_t size, char* path){
        #ifdef DEBUG
            std::cout << "Header: " << header.fileName.data() << " Size: " << size << " Path: " << path << std::endl;             
        #endif
    };

    int addPath(std::string& path);
    
    int openTar(std::string path);
    int closeTar();

    int unpackTar(std::string pathToArhive, std::string ExtractToPath);

    int setBlockSizeWrite(size_t bytes);

    void setCryptoModule(std::string pathToModule, std::string key, size_t sizeKey);
    void disableCryptoModule();
private:


    int configObj(std::string& path, const struct stat& stat, std::string px);
    int configFile(std::string& path, const struct stat& stat, std::string px);
    int configDir(std::string& path, const struct stat& stat, std::string px);

    int writeHeaderTar(const char* const buffer, const size_t bytesCount);
    int writeDataFile(std::string& path, const size_t sizeFile, TarHeader& header, size_t skipHash);
    int writeExpend512BYTES(const size_t countZeros);


    int readFileWithProgress(FILE* file, FILE* fileExtract, size_t bytes, TarHeader& header);

    int readTarHeader(FILE* file, TarHeader& header);
    void printTarHeader(TarHeader& header);



    std::string getHexLength(const std::string& str);

    std::string insertStringAfterLastSlash(const std::string& originalString, 
                                           const std::string& insertString);

    std::string trimString(const std::string& str1, const std::string str2);


    std::string fileName(std::string& path, std::string str);
    std::string trimStringFromFolder(const std::string& str, const std::string& folder);

    std::string findFromTo(std::string& str, std::string from, std::string to);

    int addPathTable(std::string& path);
    std::unordered_map<std::string, int> filePaths; // Хэш-таблица для хранения путей и значений (1 или другого)
  
    time_t convertHexToTime(const std::array<char, 12>& hexTime);

    bool flag_absolute_names = false;
    bool flag_strip_path = false;
    std::string strip_path;

    size_t countFilesSec = 0;

    size_t blockSizeWrite = 4096;

    std::string pathToModule;
    std::string key;
    size_t sizeKey = 0;

    FILE* tarFile = nullptr;
};
