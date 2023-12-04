#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "cryptotarlib.hpp"
int main(){


    std::cout << "TEST CRYPTOLIB" << std::endl;


    std::string file1 = "file1";
    std::string file2 = "file2";
    std::string file3 = "file3";
    std::string file4 = "file4";
    std::string file5 = "file5";
    std::string file6 = "file6";

    std::string file7 = "dir1";

    std::string file8 = "dir2Real/";

    std::system("mkdir dir1");
    std::system("mkdir dir2Real");
    std::system("dd if=/dev/urandom of=file1 bs=631 count=1");
    std::system("dd if=/dev/urandom of=file2 bs=431 count=1");
    std::system("dd if=/dev/urandom of=file3 bs=2214 count=1");
    std::system("dd if=/dev/urandom of=file4 bs=435 count=1");
    std::system("dd if=/dev/urandom of=file5 bs=354 count=1");
    std::system("dd if=/dev/urandom of=file6 bs=2356 count=1");


    std::system("dd if=/dev/urandom of=dir1/file1 bs=1426 count=1");
    std::system("dd if=/dev/urandom of=dir1/file2 bs=492 count=1");


    std::system("dd if=/dev/urandom of=dir2Real/fileT bs=526 count=1");
    std::system("dd if=/dev/urandom of=dir2Real/fileF bs=4532 count=1");

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



    std::system("rm file1");
    std::system("rm file2");
    std::system("rm file3");
    std::system("rm file4");
    std::system("rm file5");
    std::system("rm file6");
    
    std::system("rm -rf dir1");
    std::system("rm -rf dir2Real");


    std::cout << "TEST 2" << std::endl;

    cryptotar artar;
    std::system("mkdir unpack");
    artar.unpackTar("myTar.ctar", "unpack/");

    std::cout << "TEST SEC!" << std::endl;




    // std::vector<std::string> paths;
    // paths.push_back("file1");
    // paths.push_back("file2");
    // paths.push_back("dir1");
    // // paths.push_back("file3");
    // paths.push_back("/Users/daniilarsentev/project/testFolder");
    // paths.push_back("/Users/daniilarsentev/project/file1");
    // paths.push_back("/Users/daniilarsentev/project/file3");
    // cryptotar tar("arh.ctar", paths);
    // 
    // // cryptotar cr("myTar.ctar");
    // // cr.addPath(paths.at(0));
    // // cr.addPath(paths.at(1));
    // //
    // // cr.closeTar(); 
    //
    // // cryptotar tarEx("arh.ctar", "/Users/daniilarsentev/project/testFolder/new/");
    // // cryptotar tarEx;
    // // // tarEx.globalProgressCallback = customProgress;
    // // tarEx.setBlockSizeWrite(10240);
    // //
    // // tarEx.unpackTar("arh.ctar", ".");
    return 0;
}
