#include <iostream>
#include <string>
#include <vector>

#include "cryptotarlib.hpp"

int main(){
    std::vector<std::string> paths;
    paths.push_back("file1");
    paths.push_back("file2");
    paths.push_back("file3");
    //
    // cryptotar tar("arh.ctar", paths);
    
    // cryptotar cr("myTar.ctar");
    // cr.addPath(paths.at(0));
    // cr.addPath(paths.at(1));
    //
    // cr.closeTar(); 

    cryptotar tarEx("myTar.ctar", ".");

    return 0;
}
