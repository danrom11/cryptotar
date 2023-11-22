#include <iostream>
#include <vector>

#include "cryptotarlib.hpp"

int main(){
    std::vector<std::string> paths;
    paths.push_back("testFolder1");

    cryptotar tar("arh.ctar", paths);

    return 0;
}
