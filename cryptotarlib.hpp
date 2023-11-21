#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <stdarg.h>

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

class cryptotar{
public:
    cryptotar();
    cryptotar(std::string archiveName);
    cryptotar(std::string archiveName, std::vector<std::string>& paths);


    ~cryptotar();


private:

    int closeTar();

    int configObj(std::string& path, const struct stat& stat, std::string px);
    int configFile(std::string& path, const struct stat& stat, std::string px);
    // int configDir(std::string& path, const struct stat& stat, std::string px);

    int writeHeaderTar(const char* const buffer, const size_t bytesCount);
    int writeDataFile(std::string& path, const size_t sizeFile);
    int writeExpend512BYTES(const size_t countZeros);



    std::string getHexLength(const std::string& str);

    std::string insertStringAfterLastSlash(const std::string& originalString, 
                                           const std::string& insertString);

    std::string trimString(const std::string& str1, const std::string str2);


    std::string fileName(std::string& path, std::string str);
    std::string trimStringFromFolder(const std::string& str, const std::string& folder);

    int addPathTable(std::string& path);
    std::unordered_map<std::string, int> filePaths; // Хэш-таблица для хранения путей и значений (1 или другого)
   
    bool flag_absolute_names = false;
    bool flag_strip_path = false;
    std::string strip_path;

    size_t countFilesSec = 0;

    FILE* tarFile = nullptr;
};
