#ifndef _ABSTRACT_LOGGER_H_
#define _ABSTRACT_LOGGER_H_

#include <string>

enum LogLevel {
    DEBUG,
    ERROR,
    INFO,
    WARNING,
    NONE
};

class AbstractLogger
{
    public:
        AbstractLogger() {}
        virtual ~AbstractLogger() {}
        virtual AbstractLogger & operator << (const std::string& str) = 0;
        virtual AbstractLogger & operator()(LogLevel level) = 0;
};


#endif /* _ABSTRACT_LOGGER_H_*/
