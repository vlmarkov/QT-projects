#include <iostream>

#include "AbstractLogger.hpp"
#include "ConsoleLogger.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    ConsoleLogger consoleLogger;
    AbstractLogger* logger = &consoleLogger;

    (*logger)          << "hello world!";
    (*logger)(DEBUG)   << "hello world!";
    (*logger)(ERROR)   << "hello world!";
    (*logger)(INFO)    << "hello world!";
    (*logger)(WARNING) << "hello world!";

    return 0;
}