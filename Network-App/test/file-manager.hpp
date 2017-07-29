#ifndef _FILE_MANAGER_HPP_
#define _FILE_MANAGER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <ios>



class AbstractFileManager
{     
    public:
        AbstractFileManager() {}
        virtual ~AbstractFileManager() {}

        virtual void writeToFile(const char* buffer, int bytes) = 0;    
};


class FileManager : public AbstractFileManager
{
    private:
        std::fstream file_;

    public:
        FileManager(std::string path, std::ios_base::openmode mode);
        ~FileManager();
        
        void writeToFile(const char* buffer, int bytes);

};

#endif // _FILE_MANAGER_HPP_
