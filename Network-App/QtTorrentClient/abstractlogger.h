#ifndef __ABSTRACT_LOGGER_H_
#define _ABSTRACT_LOGGER_H_

#include <QString>

typedef enum {
    ERR,     // Designates very severe error events that will
             // presumably lead the application to abort.

    INFO,    // Designates informational messages that highlight
             // the progress of the application at coarse-grained level.

    DEBUG,   // Designates fine-grained informational events that are
             // most useful to debug an application.

    WARNING, // Designates potentially harmful situations.
} logLevel_e;

/*
 * Declare an abstract base class for all logging procedures
 * (gui, console, file).
 */
class AbstractLogger {
    public:
        virtual void makeLog(logLevel_e level, Qstring errMsg) = 0;
};

/*
 * TODO: 1. FileLogger (logging all log levels)
 *       2. GuiLogger  (logging onle error level)
 */

#endif // _ABSTRACT_LOGGER_H_
