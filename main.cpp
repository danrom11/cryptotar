#include <iostream>
#include <vector>

#include "cryptotarlib.hpp"

int main(){
    std::vector<std::string> paths;
    paths.push_back("file1");
    paths.push_back("file2");
    paths.push_back("file3");

    cryptotar tar("arh.ctar", paths);

    return 0;
}
