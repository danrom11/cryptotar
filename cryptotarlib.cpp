#include "cryptotarlib.hpp"

cryptotar::cryptotar(){
    std::cout << "ENABLED LIB" << std::endl;
    
}

cryptotar::cryptotar(std::string archiveName){

}

cryptotar::cryptotar(std::string archiveName, std::vector<std::string>& paths){
    this->tarFile = fopen(archiveName.c_str(), "wb");
    if(tarFile == NULL){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Create TAR file: %s\n", archiveName.c_str());
        // static_assert("create tar file");
        return;
    }

    for(auto it = paths.begin(); it != paths.end(); it++){
        DEBUG_PRINT_SEC("file: %s\n", it->c_str());

        int ret;
        struct stat statObj;
        if((ret = lstat(it->c_str(), &statObj)) == -1){
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Not found lstat file: %s\n", it->c_str());
        } else {
            this->countFilesSec += configObj(*it, statObj, "/");
        }
    }

    closeTar();
}

int cryptotar::closeTar(){
    if(this->tarFile != nullptr){
        if(writeExpend512BYTES(1024) && this->countFilesSec > 0)
            DEBUG_PRINT_SEC("Tar complited%s\n", "");
        else
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Last write bytes%s\n", "");
        fclose(tarFile);
        return 1;
    }
    DEBUG_PRINT_ERR("TAR file not creared%s\n", "");
    return 0;
}





int cryptotar::configObj(std::string& path, const struct stat& statObj, std::string px){
    if(S_ISREG(statObj.st_mode)){
        DEBUG_PRINT_SEC("This file: %s\n", path.c_str());
        return configFile(path, statObj, "/");

    } else if(S_ISDIR(statObj.st_mode)){
        DEBUG_PRINT_SEC("This dir: %s\n", path.c_str());
        // return configDir(path, statObj, "/");

    } else if(S_ISLNK(statObj.st_mode)){
        DEBUG_PRINT_SEC("This link: %s\n", path.c_str());
    }
    return 0;
}


int cryptotar::configFile(std::string& path, const struct stat& statObj, std::string px){
    TarHeader header = {};

    strcpy(header.formatTar.data(), "cryptotar\0");
    
    if(__APPLE__){
        strcpy(header.version.data(), "00\0");
        //std::cout << "APPLE" << std::endl;
    } else {
        strcpy(header.version.data(), " 0\0");
    }

    //TODO: DEL
    std::string nameFile;
    if(path[0] == '/' && flag_absolute_names == false){
        nameFile = fileName(path, px);
        px = trimStringFromFolder(path, nameFile);
    }
    else {
        if(flag_strip_path)
            nameFile = trimString(path, strip_path);
        else
            nameFile = path;
    }

    //addPathTable(nameFile);

    TarHeader::decToHexStr(header.uid, statObj.st_uid);
    TarHeader::decToHexStr(header.gid, statObj.st_gid);
    TarHeader::decToHexStr(header.mtime, statObj.st_mtime, 1);
    TarHeader::decToHexStr(header.atime, statObj.st_atime);
    TarHeader::decToHexStr(header.ctime, statObj.st_ctime);
    TarHeader::decToHexStr(header.mode, statObj.st_mode & ACCESSPERMS);

    TarHeader::decToHexStr(header.devmajor, major(statObj.st_dev));
    TarHeader::decToHexStr(header.devminor, minor(statObj.st_dev));

    if(const struct passwd* const pw = getpwuid(statObj.st_uid))
        strcpy(header.uname.data(), pw->pw_name);
    if(const struct group* const grp = getgrgid(statObj.st_gid))
        strcpy(header.gname.data(), grp->gr_name);

    strncpy(header.fileName.data(), nameFile.data(), std::min(header.fileName.size(), nameFile.size()));
    header.typeFlag = TarHeader::getTypeFlag(statObj.st_mode);
    DEBUG_PRINT_SEC("data name: %s\n", header.fileName.data());



    //header.calcChecksum();
    if(writeHeaderTar((const char*)&header, sizeof(TarHeader))){
        DEBUG_PRINT_SEC("FILE: %s SEC HEADER WRITE!\n", path.c_str());
        std::string body;

        const std::string pathToFile = " path=" + path + '\n';
        body += getHexLength(pathToFile) + pathToFile;
        
        std::ostringstream ostr;
        ostr<<statObj.st_size;
        std::string size_str = ostr.str();
        const std::string file_size = " size=" + size_str + '\n';
        body += getHexLength(file_size) + file_size;

        if(writeHeaderTar(body.c_str(), body.size()))
            DEBUG_PRINT_SEC("FILE: %s SEC BODY WRITE!\n", path.c_str());

        if(writeDataFile(path, statObj.st_size))
            DEBUG_PRINT_SEC("FILE: %s SEC DATA WRITE!\n", path.c_str());
    }

    return 1;
}


int cryptotar::writeHeaderTar(const char* const buffer, const size_t bytesCount){
    size_t bytesWritten = fwrite(buffer, 1, bytesCount, tarFile);
    if(bytesCount != bytesWritten){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Write bytes to TAR Header\n%s", "");
        return 0;
    }

    return 1;
}

int cryptotar::writeDataFile(std::string& path, const size_t sizeFile){
    FILE* file = fopen(path.c_str(), "rb+");
    if(file == NULL){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Open file: %s\n", path.c_str());
        return 0;
    }
    int fd = fileno(file);

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();

    if(fcntl(fd, F_SETLK, &fl) == -1){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Lock file: %s\n", path.c_str());
        return 0;
    }
    fl.l_type = F_UNLCK;

    size_t predictSize = 0;
    fseek(file, 0, SEEK_END); // Перемещаем указатель в конец файла
    predictSize = ftell(file);  // Получаем текущую позицию указателя, которая равна размеру файла
    rewind(file);

    if(sizeFile != predictSize){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: The sizes are different: size1: %zu, size2: %zu\n", sizeFile, predictSize);
        
        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        fclose(file);
        return 0;
    }

    char* buffer = new char[sizeFile];
    if(buffer == NULL){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Malloc size: %zu\n", sizeFile);

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }
    
    size_t readBytes = fread(buffer, 1, sizeFile, file);
    if(readBytes != sizeFile){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: The size are different read: %zu, size: %zu\n", readBytes, sizeFile);

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }

    size_t bytesWritten = fwrite(buffer, 1, sizeFile, tarFile);
    if(sizeFile != bytesWritten){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Write bytes to Data file\n%s", "");

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }

    uint64_t countZeros = expandSizeTo512Blocks(bytesWritten);

    DEBUG_PRINT_SEC("INFO: writeB: %zu, countZeros: %llu\n", bytesWritten, countZeros);
    
    if(!writeExpend512BYTES(countZeros)){
        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);

        return 0;
    }



    delete[] buffer;
    if(fcntl(fd, F_SETLK, &fl) == -1){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Unlock file: %s\n", path.c_str());
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}


int cryptotar::writeExpend512BYTES(const size_t countZeros){
    if(tarFile == NULL){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Write bytes ZERO\n%s", "");
        return 0;
    }
    static constexpr uint64_t COPY_BUFFER_SIZE = 1024 * 1024 * 20; //409600 * sizeof(TARHeader);
    std::vector<char> m_buffer = std::vector<char>(COPY_BUFFER_SIZE, 0);
    size_t bytesWritten = fwrite(m_buffer.data(), 1, countZeros, tarFile);

    if(bytesWritten != countZeros){
        DEBUG_PRINT_ERR("OXYUSTAR_ERROR: Write bytes ZERO deifferent: %zu | %zu\n", countZeros, bytesWritten);
        return 0;
    }
    return 1;
}

std::string cryptotar::getHexLength(const std::string& str){
    size_t size = str.size(); // +3, так как скорее всего длина заголовка вместе с цифрой трёхзначное число.
    std::stringstream ss;
    
    // Конвертируем размер в шестнадцатеричную систему счисления
    ss << std::hex << size;

    std::string length = ss.str();

    // Проверяем, достигнута ли необходимая длина
    while (length.size() + str.size() != size) {
        ss.str("");  // Очищаем поток
        ss << std::hex << ++size;
        length = ss.str();
    }

    return length;
}

std::string cryptotar::fileName(std::string& path, std::string str){
    
    size_t lastSub = std::string::npos;
    lastSub =  path[0] == '/' ? path.rfind(str) : path.find(str);
    if(lastSub != std::string::npos){
        return path.substr(lastSub + str.length());
    } 
    return path;
}


std::string cryptotar::trimStringFromFolder(const std::string& str, const std::string& folder)
{
    std::size_t pos = str.find(folder);
    
    if (pos != std::string::npos)
    {
        // Обрезаем строку начиная от начала до позиции найденной подстроки
        return str.substr(0, pos);
    }
    
    return str;
}



std::string cryptotar::insertStringAfterLastSlash(const std::string& originalString, const std::string& insertString){
    std::string modifiedString = originalString;
    size_t lastSlashPos = modifiedString.find_last_of('/');

    if (lastSlashPos != std::string::npos) {
        modifiedString.insert(lastSlashPos+1, insertString);
    } else {
        modifiedString.insert(0, insertString);
    }

    return modifiedString;
}


int cryptotar::addPathTable(std::string& path){
    auto res = filePaths.emplace(path, 1);
    if(!res.second){
        path += "_";
        // if (!path.empty() && path.back() == '/') {
        //     path.insert(path.length() - 1, "_");
        // } else {
        //     path += "_";
        // }
       
        DEBUG_PRINT_SEC("new path: %s\n", path.c_str());
        addPathTable(path);
    }
    return 1;
}


std::string cryptotar::trimString(const std::string& str1, const std::string str2){
    if(str1.size() > str2.size()){
        return str1.substr(str2.size(), str1.size() - str2.size());
    } else {
        return str1;
    }
}


cryptotar::~cryptotar(){
    std::cout << "DEST LIB" << std::endl;
}
