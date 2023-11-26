#include <iostream>
#include <string>
#include <vector>

#include "cryptotarlib.hpp"
void customProgress(size_t bytesRead, size_t fileSize) {
    // Пользовательская логика
    std::cout << "TEST: " << bytesRead << std::endl;
}
int main(){
    // std::vector<std::string> paths;
    // paths.push_back("file1");
    // paths.push_back("dir1");
    // paths.push_back("file3");
    // cryptotar tar("arh.ctar", paths);
    
    // cryptotar cr("myTar.ctar");
    // cr.addPath(paths.at(0));
    // cr.addPath(paths.at(1));
    //
    // cr.closeTar(); 

    // cryptotar tarEx("arh.ctar", "/Users/daniilarsentev/project/testFolder/new/");
    cryptotar tarEx;
    tarEx.globalProgressCallback = customProgress;

    tarEx.unpackTar("arh.ctar", "/Users/daniilarsentev/project/testFolder/new/");
    return 0;
}
