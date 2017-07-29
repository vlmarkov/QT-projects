#ifndef _CONSOLE_LOGGER_H_
#define _CONSOLE_LOGGER_H_

#include <string>
#include <sstream>
#include <iostream>

#include "AbstractLogger.hpp"


class ConsoleLogger : public AbstractLogger
{
    private:
        LogLevel           level_;
        std::ostream*      ostream_;

        std::string logInfo();
        std::string logGetTime();
        std::string logLevelResolve();

    public:
        ConsoleLogger(std::ostream& rhs = std::clog);
        ~ConsoleLogger();

        ConsoleLogger & operator << (const std::string& str);
        ConsoleLogger & operator()(LogLevel level);
    
};


#endif /* _CONSOLE_LOGGER_H_*/
