#include <iostream>

#include "file-manager.hpp"


int main(int argc, char const *argv[])
{
    FileManager* manager = new FileManager("test.txt", std::fstream::out);

    AbstractFileManager *fileManager = &*manager;

    try {
        std::string str = ("hello world - 1.1\n");
        fileManager->writeToFile(str.c_str(), str.size());
        fileManager->writeToFile(str.c_str(), str.size());
    } catch (std::string err) {
        std::cerr << err << std::endl;
        return -1;
    }

    delete fileManager;

    return 0;
}