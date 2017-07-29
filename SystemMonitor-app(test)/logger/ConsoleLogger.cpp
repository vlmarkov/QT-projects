#include "ConsoleLogger.hpp"

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

ConsoleLogger::ConsoleLogger(std::ostream& rhs) : level_(NONE), ostream_(&rhs) { }

ConsoleLogger::~ConsoleLogger() { /* TODO */ }

ConsoleLogger & ConsoleLogger::operator << (const std::string& str)
{
    *this->ostream_ << this->logInfo() << str << std::endl;

    return *this;
}

ConsoleLogger & ConsoleLogger::operator()(LogLevel level)
{
    this->level_ = level;

    return *this;
}

std::string ConsoleLogger::logInfo()
{
    return std::string("<" + this->logGetTime() + "> " + this->logLevelResolve() + " ");
}

std::string ConsoleLogger::logGetTime()
{
    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string curTime(ctime(&time));
    curTime.resize(curTime.size()-1);

    return curTime;
}


std::string ConsoleLogger::logLevelResolve()
{
    switch(this->level_)
    {
        case DEBUG   : return std::string("[ DEBUG ]");
        case ERROR   : return std::string("[ ERROR ]");
        case INFO    : return std::string("[ INFO  ]");
        case WARNING : return std::string("[WARNING]");
        default      : return std::string("[       ]");
    }
}
