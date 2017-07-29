#include "file-manager.hpp"


FileManager::FileManager(std::string path, std::ios_base::openmode mode)
{
    this->file_.open(path.c_str(), mode);

    if (!this->file_.is_open())
        throw std::string(std::string("Can't open file by path: ") + path);
}

FileManager::~FileManager()
{ 
    this->file_.close();
}

void FileManager::writeToFile(const char* buffer, int bytes)
{
    this->file_.write(buffer, bytes);
}
