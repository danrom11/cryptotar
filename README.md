<p align="center">
      <img src="https://sun9-76.userapi.com/impg/9v7gxtOn0-hZigN_A-5vt0boqzUNaPeIW4WXWg/9_sZodPSDv8.jpg?size=1024x1024&quality=95&sign=0d8154a894c5ba6f58d66384b5ced277&type=album" width="256">
</p>

<p align="center">
   <img src="https://img.shields.io/badge/VERSION-0.1-green" alt="Version">
   <img src="https://img.shields.io/badge/LICENSE-MIT-blue" alt="License">
</p>

<p align="center">
   <img src="https://img.shields.io/badge/BUILD-0.1-green?logo=linux" alt="Build linux" width="128">
   <img src="https://img.shields.io/badge/BUILD-0.1-green?logo=macOS" alt="Build macos" width="128">
   <img src="https://img.shields.io/badge/BUILD-%E2%9C%98-red?logo=windows" alt="Build windows" width="128">
</p>

## About

We are excited to announce a significant upgrade to the traditional TAR archive format. Our new release, dubbed "Cryptographic  TAR" (CRYPTOTAR), introduces a groundbreaking approach to secure data archiving. This enhanced format is specifically designed to meet the growing demand for robust security in data storage and transmission.

Key Features of CRYPTOTAR:

  -  Modular Encryption: CRYPTOTAR leverages a modular encryption design, allowing users to choose from a variety of encryption algorithms based on their specific needs and security policies. This flexibility ensures that the archive format remains agile and adaptable to future cryptographic standards and advancements.

  -  Enhanced Security: Each file within a CRYPTOTAR archive is individually encrypted, providing an additional layer of security. This method not only protects the entire archive but also secures each file against unauthorized access, even if the archive is partially compromised.

  -  Backward Compatibility: While CRYPTOTAR introduces new security features, it maintains backward compatibility with the traditional TAR format. This ensures seamless integration into existing workflows and systems, allowing users to upgrade without disrupting their current operations.

  -  Efficient Compression: Alongside encryption, CRYPTOTAR supports efficient compression techniques to reduce the size of the archives. This feature is particularly beneficial for large datasets, enabling users to save on storage costs and accelerate data transfer rates.

  -  Integrity Checks: CRYPTOTAR incorporates advanced integrity checking mechanisms. These checks ensure that the data has not been tampered with, providing an extra layer of security and trust in the integrity of the archived data.

  -  User-Friendly Interface: Despite its advanced features, CRYPTOTAR is designed with a user-friendly interface. It simplifies the process of creating, managing, and extracting encrypted archives, making it accessible to both technical and non-technical users.

  -  Open Source Development: Embracing the open-source model, CRYPTOTAR development and updates are driven by a community of dedicated developers and security experts. This collaborative approach ensures continuous improvement and adaptation to the evolving cybersecurity landscape.

  -  CRYPTOTAR represents a significant step forward in secure data archiving, catering to the modern-day requirements of data security and privacy. Whether for personal use or enterprise-level applications, CRYPTOTAR offers a robust, flexible, and user-friendly solution for secure data storage and transmission.

## Quick Start

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

6. Library Features.

   You can read more features of the library in the library documentation at links
