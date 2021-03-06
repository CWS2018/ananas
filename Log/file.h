#ifndef FILE_H_
#define FILE_H_

#include "./noncopy.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

class File : noncopy
{
public:
    File() : _fp(nullptr) {}
    ~File() {
        this->close();
    }

    void open(const std::string &filename);
    void close() {
        if(_fp)
            fclose(_fp);
    }
    void flush();
    void append(const char *msg, int len);
    void reset(const std::string &filename);
private:
    FILE *_fp;
    char _buf[1024];
};

#endif