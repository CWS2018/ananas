#ifndef LOGFILE_H_
#define LOGFILE_H_

#include "./noncopy.h"

#include <string>

class LogFile : noncopy
{
public:
    explicit LogFile(const std::string &filename);
    ~LogFile();
private:
    std::string _filename;
};

#endif