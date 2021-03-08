#ifndef LOGSTREAM_H_
#define LOGSTREAM_H_

#include "./logbuffer.h"

#include <string>

const int ksmallbuffer = 4000;
//const int klargebuffer = 4000 * 1000;

class LogStream
{
public:
    typedef LogStream self;
    typedef LogBuffer<ksmallbuffer> Buffer;

    LogStream() = default;
    ~LogStream() {}

    // 重载 << 运算符
    self& operator<<(short v);
    self& operator<<(unsigned short v);
    self& operator<<(int v);
    self& operator<<(unsigned int v);
    self& operator<<(long v);
    self& operator<<(unsigned long v);
    self& operator<<(long long v);
    self& operator<<(unsigned long long v);
    //self& operator<<(float v);
    //self& operator<<(double v);
    //self& operator<<(long double v);

    self& operator<<(const char v);
    self& operator<<(const char *v);
    self& operator<<(const std::string &v);

    // 
    const Buffer& buffer() const {
        return _buffer;
    }
private:
    template<typename T>
    void formatInteger(T v);

    Buffer _buffer;

    static const int kintegerSize = 20;
};

#endif