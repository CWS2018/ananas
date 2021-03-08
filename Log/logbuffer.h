#ifndef LOGBUFFER_H_
#define LOGBUFFER_H_

#include <string.h>
#include <iostream>

template<int SIZE>
class LogBuffer
{
public:
    LogBuffer() : _cur(_data) {}
   // LogBuffer(const LogBuffer &buf);
    ~LogBuffer() {}

    void bzero() {
        memset(_data, 0, sizeof(_data));
    }

    void add(int len) {
        _cur += len;
    }

    char* current() {
        return _cur;
    }

    const char* end() {
        return (_data + sizeof(_data));
    }

    int avail() {
        return static_cast<int>(end() - _cur);
    }

    void append(const char *data, int len) {
        if(avail() > len) {
            memcpy(_cur, data, len);
            _cur += len;
        }
    }

    void reset() {
        _cur = _data;
    }

    const char* data() const {
        return _data;
    }

    int length() const {
        return static_cast<int>(_cur - _data);
    }
private:
    char _data[SIZE];
    char *_cur;
};

#endif