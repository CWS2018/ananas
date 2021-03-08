#ifndef ASYNCLOG_H_
#define ASYNCLOG_H_

#include "./logbuffer.h"
#include "./noncopy.h"
#include "./mutex.h"
#include "./condition.h"
#include "./thread.h"
#include "./countDownLatch.h"

#include <string>
#include <atomic>
#include <vector>
#include <memory>

const int klargebuffer = 4000 * 100;

class AsyncLog : noncopy
{
public:
    typedef LogBuffer<klargebuffer> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;

    //AsyncLog();
    AsyncLog(const std::string logfilename, int seconds = 2);
    ~AsyncLog();

    void start() {
        _working = true;
        _thread.start();
        _count.wait();
    }

    void stop() {
        _working = false;
        _cond.notify();
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
    countDownLatch _count;
    Thread _thread;
};


#endif