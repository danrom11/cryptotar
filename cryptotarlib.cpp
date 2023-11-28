#include "cryptotarlib.hpp"
#include "sha256.h"

cryptotar::cryptotar(std::string archiveName){
    this->tarFile = fopen(archiveName.c_str(), "wb");
    if(tarFile == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Create TAR file: %s\n", archiveName.c_str());
        // static_assert("create tar file");
        return;
    }
}

cryptotar::cryptotar(std::string archiveName, std::vector<std::string>& paths){
    this->tarFile = fopen(archiveName.c_str(), "wb");
    if(tarFile == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Create TAR file: %s\n", archiveName.c_str());
        // static_assert("create tar file");
        return;
    }

    for(auto it = paths.begin(); it != paths.end(); it++){
        DEBUG_PRINT_SEC("file: %s\n", it->c_str());

        int ret;
        struct stat statObj;
        if((ret = lstat(it->c_str(), &statObj)) == -1){
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Not found lstat file: %s\n", it->c_str());
        } else {
            this->countFilesSec += configObj(*it, statObj, "/");
        }
    }

    closeTar();
}

int cryptotar::addPath(std::string& path){
    if(this->tarFile == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Tar file not created:%s\n", "");
    }

    int ret;
    struct stat statObj;
    if((ret = lstat(path.c_str(), &statObj)) == -1){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Not found lstat file: %s\n", path.c_str());
        return 0;
    } else {
        this->countFilesSec += configObj(path, statObj, "/");
        return 1;
    }
}

int cryptotar::closeTar(){
    if(this->tarFile != nullptr){
        if(writeExpend512BYTES(1024) && this->countFilesSec > 0)
            DEBUG_PRINT_SEC("Tar complited%s\n", "");
        else
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Last write bytes%s\n", "");
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
        return configDir(path, statObj, "/");

    } else if(S_ISLNK(statObj.st_mode)){
        DEBUG_PRINT_SEC("This link: %s\n", path.c_str());
    }
    return 0;
}


int cryptotar::configFile(std::string& path, const struct stat& statObj, std::string px){
    TarHeader header = {};

    strcpy(header.formatTar.data(), "cryptotar\0");
    strcpy(header.version.data(), VERSION);

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

    addPathTable(nameFile);

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

    header.calcChecksum(sha256_file(path).data());

    if(writeHeaderTar((const char*)&header, sizeof(TarHeader))){
        DEBUG_PRINT_SEC("FILE: %s SEC HEADER WRITE!\n", path.c_str());
        std::string body;

        const std::string pathToFile = " path=" + (path[0] == '/' ? nameFile : (path == nameFile) ? path : nameFile) + '\n';
        body += getHexLength(pathToFile) + pathToFile;
        
        std::ostringstream ostr;
        ostr<<statObj.st_size;
        std::string size_str = ostr.str();
        const std::string file_size = " size=" + size_str + '\n';
        body += getHexLength(file_size) + file_size;

        // if(writeHeaderTar(body.c_str(), body.size()))
        //     DEBUG_PRINT_SEC("FILE: %s SEC BODY WRITE!\n", path.c_str());

        size_t bytesWritten = fwrite(body.c_str(), 1, body.size(), tarFile);
        if(body.size() != bytesWritten){
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: write bytes to TAR Header EXTRA%s\n", "");
        }

        size_t blocks = expandSizeTo512Blocks(bytesWritten);
        DEBUG_PRINT_SEC("Blocks: %lu\n", blocks);
        writeExpend512BYTES(blocks);

        if(writeDataFile(path, statObj.st_size))
            DEBUG_PRINT_SEC("FILE: %s SEC DATA WRITE!\n", path.c_str());
    }

    return 1;
}


int cryptotar::configDir(std::string& path, const struct stat& statObj, std::string px){ 
    
    std::string nameDir;
    if(path[0] == '/'){
        nameDir = fileName(path, px);
        px = trimStringFromFolder(path, nameDir);
    }
    else{
        if(flag_strip_path)
            nameDir = trimString(path, strip_path);
        else
            nameDir = path;
    }

    DEBUG_PRINT_SEC("path=%s\n", path.c_str());
    DEBUG_PRINT_SEC("nameDir=%s\n", nameDir.c_str());


    if(path.back() != '/'){
        path.push_back('/');
        nameDir.push_back('/');
    }

    TarHeader header = {};

    strcpy(header.formatTar.data(), "cryptotar\0");
    strcpy(header.version.data(), VERSION);


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

    strncpy(header.fileName.data(), nameDir.data(), std::min(header.fileName.size(), nameDir.size()));
    header.typeFlag = TarHeader::getTypeFlag(statObj.st_mode);
    // std::cout << "data name: " << header.name.data() << std::endl;
    DEBUG_PRINT_SEC("data name: %s\n", header.fileName.data());
    
    header.calcChecksum(nullptr);

    if(writeHeaderTar((const char*)&header, sizeof(TarHeader))){
        DEBUG_PRINT_SEC("DIR: %s SEC HEADER WRITE!\n", path.c_str());
        
        std::string body;

        const std::string pathToFile = " path=" + (path[0] == '/' ? nameDir : path) + '\n';
        body += getHexLength(pathToFile) + pathToFile;
        
        std::ostringstream ostr;
        ostr<<statObj.st_size;
        std::string size_str = ostr.str();
        const std::string file_size = " size=" + size_str + '\n';
        body += getHexLength(file_size) + file_size;

        // if(writeHeaderTar(body.c_str(), body.size()))
        //     DEBUG_PRINT_SEC("FILE: %s SEC BODY WRITE!\n", path.c_str());
        // else
        //     DEBUG_PRINT_ERR("ERR\n", "");
        size_t bytesWritten = fwrite(body.c_str(), 1, body.size(), tarFile);
        if(body.size() != bytesWritten){
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: write bytes to TAR Header EXTRA%s\n", "");
        }

        size_t blocks = expandSizeTo512Blocks(bytesWritten);

        writeExpend512BYTES(blocks);
    } else {
        DEBUG_PRINT_ERR("DIR: %s ERR HEADER WRITE!\n", path.c_str());
    }
    
    DIR* dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        // читаем все файлы в директории
        while ((ent = readdir(dir)) != NULL) {
            // проверяем, что это не директория
            if (ent->d_type == DT_REG) {
                std::string fileName = path + ent->d_name; 
                DEBUG_PRINT_SEC("Dir to file: %s\n", fileName.c_str());

                int ret;
                struct stat statObjFile;
                if((ret = lstat(fileName.c_str(), &statObjFile)) == -1){
                    DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Not found lstat file: %s\n", fileName.c_str());
                    static_assert("stat");
                    break;
                }
                configFile(fileName, statObjFile, px);
            }
            else if(ent->d_type == DT_DIR){
                if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                    std::string DirName = path + ent->d_name; 
                    DEBUG_PRINT_SEC("Dir to dir: %s\n", DirName.c_str());
                    
                    int ret;
                    struct stat statObjFile;
                    if((ret = lstat(DirName.c_str(), &statObjFile)) == -1){
                        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Not found lstat file: %s\n", DirName.c_str());
                        static_assert("stat");
                        break;
                    }
                    configDir(DirName, statObjFile, px);
                }
                

            }
        }

        closedir(dir); // закрываем директорию
    } else {
        // обработка ошибки, если не удалось открыть папку
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Open folder\n%s", "");
        return EXIT_FAILURE;
    }

    return 1;
}


int cryptotar::writeHeaderTar(const char* const buffer, const size_t bytesCount){
    size_t bytesWritten = fwrite(buffer, 1, bytesCount, tarFile);
    if(bytesCount != bytesWritten){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Write bytes to TAR Header\n%s", "");
        return 0;
    }

    return 1;
}

int cryptotar::writeDataFile(std::string& path, const size_t sizeFile){
    FILE* file = fopen(path.c_str(), "rb+");
    if(file == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Open file: %s\n", path.c_str());
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
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Lock file: %s\n", path.c_str());
        return 0;
    }
    fl.l_type = F_UNLCK;

    size_t predictSize = 0;
    fseek(file, 0, SEEK_END); // Перемещаем указатель в конец файла
    predictSize = ftell(file);  // Получаем текущую позицию указателя, которая равна размеру файла
    rewind(file);

    if(sizeFile != predictSize){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: The sizes are different: size1: %zu, size2: %zu\n", sizeFile, predictSize);
        
        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        fclose(file);
        return 0;
    }

    char* buffer = new char[sizeFile];
    if(buffer == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Malloc size: %zu\n", sizeFile);

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }
    
    size_t readBytes = fread(buffer, 1, sizeFile, file);
    if(readBytes != sizeFile){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: The size are different read: %zu, size: %zu\n", readBytes, sizeFile);

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }

    size_t bytesWritten = fwrite(buffer, 1, sizeFile, tarFile);
    if(sizeFile != bytesWritten){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Write bytes to Data file\n%s", "");

        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);
        return 0;
    }

    uint64_t countZeros = expandSizeTo512Blocks(bytesWritten);

    DEBUG_PRINT_SEC("INFO: writeB: %zu, countZeros: %llu\n", bytesWritten, countZeros);
    
    if(!writeExpend512BYTES(countZeros)){
        if(fcntl(fd, F_SETLK, &fl) == -1)
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        delete[] buffer;
        fclose(file);

        return 0;
    }



    delete[] buffer;
    if(fcntl(fd, F_SETLK, &fl) == -1){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Unlock file: %s\n", path.c_str());
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}


int cryptotar::writeExpend512BYTES(const size_t countZeros){
    if(tarFile == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Write bytes ZERO\n%s", "");
        return 0;
    }
    static constexpr uint64_t COPY_BUFFER_SIZE = 1024 * 1024 * 20; //409600 * sizeof(TARHeader);
    std::vector<char> m_buffer = std::vector<char>(COPY_BUFFER_SIZE, 0);
    size_t bytesWritten = fwrite(m_buffer.data(), 1, countZeros, tarFile);

    if(bytesWritten != countZeros){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Write bytes ZERO deifferent: %zu | %zu\n", countZeros, bytesWritten);
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








cryptotar::cryptotar(std::string pathToArhive, std::string ExtractToPath){
    unpackTar(pathToArhive, ExtractToPath);
}

int cryptotar::unpackTar(std::string pathToArhive, std::string ExtractToPath){
    FILE* file = fopen(pathToArhive.c_str(), "rb");
    if(file == NULL){
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Open cryptotar file: %s\n", pathToArhive.data());
        return 0;
    }

    int blocksEnd = 0;

    while(blocksEnd != 2){
        TarHeader header;
        readTarHeader(file, header);
        
        if(header.fileName.at(0) == 0x0){
            std::cout << "Unpacking is complete!" << std::endl;
            break;
        }

        printTarHeader(header);
        
        if(std::string(header.formatTar.data()) == "cryptotar\0"){
            if(std::string(header.version.data()) != VERSION){
                std::cerr << "The file version is "<< header.version.data() << " and you have " <<
                    VERSION << " installed" << std::endl;
                return 0;
            }
        } else {
            std::cerr << "This file is not cryptotar" << std::endl;
            return 0;
        }

        char ch;
        int newlineCount = 0;
        std::string dataUntilSecondNewline;

        while ((ch = getc(file))) {
            dataUntilSecondNewline.push_back(ch);
            if (ch == '\n') {
                newlineCount++;
                if (newlineCount == 2) {
                    break;
                }
            }
        }

        DEBUG_PRINT_SEC("pax: %s\n", dataUntilSecondNewline.data());
        size_t size = 0;

        size = std::stoull(findFromTo(dataUntilSecondNewline, "size=", "\n"));
        std::string path = findFromTo(dataUntilSecondNewline, "path=", "\n");
        
        DEBUG_PRINT_SEC("size=%lu\n", size);
        DEBUG_PRINT_SEC("path=%s\n", path.data());
        
        fseek(file, 512 - dataUntilSecondNewline.size(), SEEK_CUR);
   
        if(ExtractToPath != "."){
            path.insert(0, ExtractToPath);
            DEBUG_PRINT_SEC("New path: %s\n", path.data());
        }

        if(header.typeFlag == TarHeader::TYPELAGS::REGTYPE){

            FILE* fileExtract = fopen(path.c_str(), "wb");
            if(fileExtract == NULL){
                DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Create file %s\n", header.fileName.data());
                return 0;
            }
            readFileWithProgress(file, fileExtract, size);
            fclose(fileExtract);

            size_t skeepNextBlock = expandSizeTo512Blocks(size);
            DEBUG_PRINT_SEC("Blocks: %lu\n", skeepNextBlock);
            fseek(file, skeepNextBlock, SEEK_CUR); 
        } else if(header.typeFlag == TarHeader::TYPELAGS::DIRTYPE){
            path.pop_back();
            mkdir(path.c_str(), 0777);
        }



        DEBUG_PRINT_SEC("--------------------------------------------------%s\n", "");
    }
    fclose(file);
    return 1;
}


int cryptotar::readTarHeader(FILE* file, TarHeader& header){
    int statusRead = 0;
    statusRead += fread(header.fileName.data(), 1, 100, file);
    statusRead += fread(header.formatTar.data(), 1, 10, file);
    statusRead += fread(header.mode.data(), 1, 8, file);
    statusRead += fread(header.uid.data(), 1, 8, file);
    statusRead += fread(header.gid.data(), 1, 8, file);
    statusRead += fread(header.mtime.data(), 1, 12, file);
    statusRead += fread(header.chksum.data(), 1, 65, file);
    char  typeFlag;
    statusRead += fread(&typeFlag, 1, 1, file);
    header.typeFlag = static_cast<TarHeader::TYPELAGS>(typeFlag);
    
    statusRead += fread(header.uname.data(), 1, 32, file);
    statusRead += fread(header.gname.data(), 1, 32, file);
    statusRead += fread(header.devmajor.data(), 1, 8, file);
    statusRead += fread(header.devminor.data(), 1, 8, file);
    statusRead += fread(header.atime.data(), 1, 12, file);
    statusRead += fread(header.ctime.data(), 1, 12, file);
    statusRead += fread(header.version.data(), 1, 2, file);
    statusRead += fread(header.prefix.data(), 1, 194, file);

    return statusRead;
}

void cryptotar::printTarHeader(TarHeader& header){
    DEBUG_PRINT_SEC("fileName: %s\n", header.fileName.data());
    DEBUG_PRINT_SEC("formatTar: %s\n", header.formatTar.data());
    DEBUG_PRINT_SEC("mode: %s\n", header.mode.data());
    DEBUG_PRINT_SEC("uid: %s\n", header.uid.data());
    DEBUG_PRINT_SEC("gid: %s\n", header.gid.data());
    DEBUG_PRINT_SEC("mtime: %s\n", header.mtime.data());
    DEBUG_PRINT_SEC("chksum: %s\n", header.chksum.data());
    DEBUG_PRINT_SEC("typeFlag: %c\n", static_cast<char>(header.typeFlag));
    DEBUG_PRINT_SEC("uname: %s\n", header.uname.data());
    DEBUG_PRINT_SEC("gname: %s\n", header.gname.data());
    DEBUG_PRINT_SEC("devmajor: %s\n", header.devmajor.data());
    DEBUG_PRINT_SEC("devminor: %s\n", header.devminor.data());
    DEBUG_PRINT_SEC("atime: %s\n", header.atime.data());
    DEBUG_PRINT_SEC("ctime: %s\n", header.ctime.data());
    DEBUG_PRINT_SEC("version: %s\n", header.version.data());
    DEBUG_PRINT_SEC("prefix: %s\n", header.prefix.data());
}


int cryptotar::readFileWithProgress(FILE* fileTar, FILE* fileExtract, size_t totalBytesToRead){
    std::vector<char> buffer(blockSizeWrite);
    size_t totalBytesRead = 0;

    while (totalBytesRead < totalBytesToRead) {
        // Вычисляем размер следующего блока
        size_t bytesToRead = std::min(blockSizeWrite, totalBytesToRead - totalBytesRead);

        // Чтение блока данных
        size_t bytesRead = fread(buffer.data(), 1, bytesToRead, fileTar);
        totalBytesRead += bytesRead;
        globalProgressCallback(totalBytesRead, totalBytesToRead);
        
        // Обработка прочитанных данных
        fwrite(buffer.data(), 1, bytesRead, fileExtract);

        if (bytesRead < bytesToRead) {
             if (feof(fileTar)) {
                DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: File end%s\n", "");
            } else if (ferror(fileTar)) {
                DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: Read file%s\n", "");
            }
            break;
        }
    }
    return totalBytesRead == totalBytesToRead;
}


std::string cryptotar::findFromTo(std::string& str, std::string from, std::string to){
    size_t pos = str.find(from);

    if (pos != std::string::npos) {
        pos += from.size();
        size_t endPos = str.find(to, pos);

        if (endPos != std::string::npos) {
            return str.substr(pos, endPos - pos);
        } else {
            DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: 'abcd' not found after 'size='%s\n", "");
        }
    } else {
        DEBUG_PRINT_ERR("CRYPTOTAR_ERROR: 'size=' not found in the string%s\n", "");
    }
    return str;
}


int cryptotar::setBlockSizeWrite(size_t bytes){
    if(bytes < 1024)
        return 0;
    return blockSizeWrite = bytes;
}


cryptotar::~cryptotar(){
    if(this->tarFile == nullptr)
        fclose(tarFile);
    else
        closeTar();
}
