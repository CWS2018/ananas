#ifndef LOGFILE_H_
#define LOGFILE_H_

#include "./noncopy.h"
#include "./file.h"
//#include "./mutex.h"

#include <string>

class LogFile : noncopy
{
public:
    explicit LogFile(const std::string &filename);
    ~LogFile() {}

    void flush();
    void append(const char *msg, int len);

    //void rollfile();                              // 自动归档
private:
    std::string _filename;
    File _file;
    //Mutex _mutex;                                 // 配合自动归档使用
};

#endif