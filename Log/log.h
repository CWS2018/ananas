#ifndef LOG_H_
#define LOG_H_

#include "./logstream.h"

#include <string>

class Log
{
public:
    Log(const char *filename, int line);
    ~Log();

    LogStream &stream() {
        return _impl._logstream;
    }
private:

    class Impl
    {
    public:
        Impl(const char *filename, int line);
        void formatInput();

        LogStream  _logstream;
        std::string _basename;
        int _line;
    };

    Impl _impl;
};

#define LOG_INFO Log(__FILE__, __LINE__).stream()

#endif