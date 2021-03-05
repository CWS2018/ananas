#ifndef ASYNCLOG_H_
#define ASYNCLOG_H_

#include "./logbuffer.h"
#include "./noncopy.h"
#include "./mutex.h"
#include "./condition.h"
#include "./thread.h"

#include <string>
#include <atomic>
#include <vector>
#include <memory>

const int klargebuffer = 4000 * 1000;

class AsyncLog : noncopy
{
public:
    typedef LogBuffer<klargebuffer> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;

    AsyncLog();
    AsyncLog(const std::string logfilename, int seconds);
    ~AsyncLog();

    void start() {
        _working = true;
        _thread.start();
    }

    void stop() {
        _working = false;
        _thread.join();
    }

    void append(const char *msg, int len);
private:
    void back_end_threadFunc();

    std::atomic<bool> _working;
    std::string _logfilename;
    int _seconds;

    BufferPtr _curBuffer;
    BufferPtr _nextBuffer;
    BufferVector _buffers;

    Mutex _mutex;
    Cond _cond;
    Thread _thread;
};


#endif