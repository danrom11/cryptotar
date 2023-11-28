# Documentation
This document is the official documentation of Cryptotar and explains everything you need to know about working with the software.

---
## Navigating through documentation:
*  [Cryptotarlib](#Cryptotarlib)
    +  Installing and building the library
    +  [The possibilities of using the library](#The-possibilities-of-using-the-library)

*  Cryptotar console
  
*  Cryptotar gui



# Cryptotarlib
## Installing and building the library

1. Cloning a repository

    Use 'git' to clone the repository.

    ```zsh
    git clone https://github.com/danrom11/cryptotar.git
    cd cryptotar
    ```
2. Choosing the right branch.

   Use 'checkout' for git to go to the library branch.

   ```zsh
   git checkout lcryptotar.lib
   ```
3. Creating an assembly folder.

   Create a folder in the branch where you will build the project.

   ```zsh
   mkdir build
   cd build
   ```

4. Building a library for your OS.

   - To build a library for macOS (`MACOS`)

  
     ```zsh
     cmake ..
     cmake --build .
     ```

   - To build a library for linux, use the 'LINUX' flag (`LINUX`)

   
     ```zsh
     cmake -DCMAKE_BUILD_TYPE=Linux ..
     cmake --build .
     ```

   - You can build the library in DEBUG mode, for this you just need to add the Debug flag, an example of such a build for Linux
     

     ```zsh
     cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_TYPE=Linux ..
     cmake --build .
     ```

5. Using the library.


  - Source file `main.cpp`


    ```cpp
    #include <iostream>
    #include <string>
    #include <vector>
        
    #include "cryptotarlib.hpp"
        
    int main(){
        
        std::vector<std::string> paths;
            
        paths.push_back("file1");
        paths.push_back("file1");
        paths.push_back("dir1");
        paths.push_back("file3");
        paths.push_back("/Users/daniilarsentev/project/testFolder");
        paths.push_back("/Users/daniilarsentev/project/file1");
        paths.push_back("/Users/daniilarsentev/project/file3");
        
        cryptotar tar("arh.ctar", paths);
          
        return 0;
    }
    ```

- Assembly `main.cpp`

  - Compilation of the main program (`-I<path to library header files`)
    
    ```zsh
    g++ -c main.cpp -o main.o -std=c++17 -I/Users/daniilarsentev/project/cryptotar
    ```

  - Linking with the library (`-L<path to library>`)

    ```zsh
    g++ main.o -L. -lcryptotar -o main
    ```

  - Launching the program

    ```
    ./main
    ```

## The possibilities of using the library
### Header files, constructors and methods:

The main header file that you will use to interact with the library class is `cryptotarlib.hpp`
This header file allows you to create a prototype of the `cryptotar` class.
Basic constructors and methods of the `cryptotar` class:


 ```cpp 
    cryptotar() = default;
    cryptotar(std::string archiveName);
    cryptotar(std::string archiveName, std::vector<std::string>& paths);

    cryptotar(std::string pathToArhive, std::string ExtractToPath);

    ~cryptotar();

    ProgressCallback globalProgressCallback = [](size_t bytesRead, size_t fileSize){
        #ifdef DEBUG
            double progress = static_cast<double>(bytesRead) / fileSize * 100.0;
            std::cout << "Read: " << bytesRead << " out of " << fileSize << " bytes (" << progress << "%)" << std::endl;
        #endif
    };

    int addPath(std::string& path);
    int closeTar();

    int unpackTar(std::string pathToArhive, std::string ExtractToPath);

    int setBlockSizeWrite(size_t bytes);
 ```

- `cryptotar() = default;` - Default constructor.

- `cryptotar(std::string archiveName);` - The constructor for the `archiveName` string creates and opens an archive for writing data. You will be able to add files in the future using the `int addPath(std::string& path);` method. Don't forget to close the archive using the `closeTar();` method when you finish working with it!
    + Example 1.

      ```cpp
      std::string path1 = "file1";
      std::string path2 = "/Users/daniilarsentev/project/testFolde";
      
      cryptotar myTar("myTar.ctar");
      myTar.addPath(path1);
      myTar.addPath(path2);
      
      cr.closeTar(); 
      ```

      
- `cryptotar(std::string archiveName, std::vector<std::string>& paths);` - The constructor immediately creates an archive on the `archiveName` line and starts packing all files and directories that are specified in vector paths containers. The paths to the vector path containers to files and folders can be both relative and can have the full path to the file/directory, do not forget to write the opposite at the beginning of the full path - `/`. Attention at the end of packing files. The constructor will automatically close the archive!
  + Example 2.
    
    ```cpp
    std::vector<std::string> paths;

    paths.push_back("file1");
    paths.push_back("file1");
    paths.push_back("dir1");
    paths.push_back("file3");
    paths.push_back("/Users/daniilarsentev/project/testFolder");
    paths.push_back("/Users/daniilarsentev/project/file1");
    paths.push_back("/Users/daniilarsentev/project/file3");

    cryptotar tar("arh.ctar", paths);
    ```


- `cryptotar(std::string pathToArhive, std::string ExtractToPath);` - The constructor for unpacking the tar archive, `pathToArhive` is a string that indicates the path to the archive (it can be a relative or real path) `ExtractToPath` is a string that indicates where the unpacking should take place, you can specify as a relative path, for example, to the same directory by specifying - `.` or to one directory below, specifying - `../` you can also specify the correct path, but do not forget to specify '/' at the beginning and at the end '/' like this - '/Users/daniilarsentev/project/TestFolder/new/'.
  + Example 3.
 
    ```cpp
    cryptotar tarEx("arh.ctar", ".");
    ```


- `int unpackTar(std::string pathToArhive, std::string ExtractToPath);` - The method allows unpacking archives when initializing a default class. `pathToArhive` is a string indicating the path to the archive (it can be a relative or real path) `ExtractToPath` is a string indicating where the unpacking should take place, you can specify as a relative path, for example, to the same directory by specifying - `.` or to one directory below, by specifying - `../` you can also specify the correct path, but don't forget to specify '/' at the beginning and at the end '/' like this - '/Users/daniilarsentev/project/TestFolder/new/'.
  + Example 4.
 
    ```cpp
    cryptotar tarEx;

    tarEx.unpackTar("arh.ctar", ".");
    ```

- `ProgressCallback globalProgressCallback = [](size_t bytesRead, size_t fileSize)` - A pointer to the callback function, to which you will help specify your function to return the values of the file unpacking progress.
  + Example 5.
 
    ```cpp
    void customProgress(size_t bytesRead, size_t fileSize) {
      std::cout << "Bytes read: " << bytesRead << std::endl;
    }
    int main(){
        cryptotar tarEx;
        tarEx.globalProgressCallback = customProgress;
    
        tarEx.unpackTar("arh.ctar", ".");
        return 0;
    }
    ```

- `int setBlockSizeWrite(size_t bytes);` - The method that sets the number of bytes in the block when unpacking and writing to the archive (By default - 4096).
  + Example 6.
 
    ```cpp
    void customProgress(size_t bytesRead, size_t fileSize) {
      std::cout << "Bytes read: " << bytesRead << std::endl;
    }
    int main(){
        cryptotar tarEx;
        tarEx.globalProgressCallback = customProgress;
        tarEx.setBlockSizeWrite(16384);
    
        tarEx.unpackTar("arh.ctar", ".");
        return 0;
    }
    ```




