#include "./logstream.h"

#include <string.h>

#include <algorithm>
#include <iostream>


typedef LogStream::self self;

const char digits[] = "9876543210123456789";
const char *zero = digits + 9;

template<typename T>
int convert(char *buf, T v) {
    T temp = v;
    char *p = buf;

    do {
        int m = static_cast<int>(temp%10);
        temp /= 10;
        *p++ = zero[m];
    } while(temp != 0);

    if(v < 0) {
        *p++ = '-';
    }
    *p = '\0';

    std::reverse(buf, p);

    return static_cast<int>(p - buf);
}

template<typename T>
void LogStream::formatInteger(T v) {
    // 将数值转换成字符串

    if(_buffer.avail() > kintegerSize) {
        // _buffer 要足够容纳 kintegerSize 个字符
        int len = convert(_buffer.current(), v);
        _buffer.add(len);
    }
}

self& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

self& LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

self& LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

self& LogStream::operator<<(const char v) {
    _buffer.append(&v, 1);
    return *this;
}

self& LogStream::operator<<(const char *v) {
    if(v) {
        _buffer.append(v, strlen(v));
    }
    else {
        _buffer.append("(NULL)", 6);
    }
    return *this;
}

self& LogStream::operator<<(const std::string &v) {
    _buffer.append(v.c_str(), v.length());
    return *this;
}