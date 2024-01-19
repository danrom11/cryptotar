#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <windows.h>

#include "cryptotarlib.hpp"

void generateRandomFile(const std::string& filename, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    for(size_t i = 0; i < size; ++i) {
        char randomByte = rand() % 256;
        file.write(&randomByte, 1);
    }
}

int main() {
    std::cout << "TEST CRYPTOLIB" << std::endl;

    std::string file1 = "file1";
    std::string file2 = "file2";
    std::string file3 = "file3";
    std::string file4 = "file4";
    std::string file5 = "file5";
    std::string file6 = "file6";
    std::string file7 = "dir1";
    std::string file8 = "dir2Real";

    CreateDirectory("dir1", NULL);
    CreateDirectory("dir2Real", NULL);

    srand((unsigned)time(NULL)); // Инициализация генератора случайных чисел

    generateRandomFile("file1", 631);
    generateRandomFile("file2", 431);
    generateRandomFile("file3", 2214);
    generateRandomFile("file4", 435);
    generateRandomFile("file5", 354);
    generateRandomFile("file6", 2356);
    generateRandomFile("dir1/file1", 1426);
    generateRandomFile("dir1/file2", 492);
    generateRandomFile("dir2Real/fileT", 526);
    generateRandomFile("dir2Real/fileF", 4532);

    cryptotar tar("myTar.ctar");
    tar.addPath(file1);
    tar.addPath(file2);
    tar.addPath(file3);
    tar.addPath(file4);
    tar.addPath(file5);
    tar.addPath(file6);
    tar.addPath(file7);
    tar.addPath(file8);
    tar.closeTar();

    DeleteFile("file1");
    DeleteFile("file2");
    DeleteFile("file3");
    DeleteFile("file4");
    DeleteFile("file5");
    DeleteFile("file6");
    RemoveDirectory("dir1");
    RemoveDirectory("dir2Real");

    std::cout << "TEST 2" << std::endl;

    cryptotar artar;
    CreateDirectory("unpack", NULL);
    artar.unpackTar("myTar.ctar", "unpack/");

    std::cout << "TEST SEC!" << std::endl;

    return 0;
}
