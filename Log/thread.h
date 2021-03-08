#ifndef THREAD_H_
#define THREAD_H_

#include "./noncopy.h" 
#include "./mutex.h"
#include "./condition.h"
#include "./countDownLatch.h"

#include <pthread.h>

#include <functional>
#include <string>

class Thread : noncopy
{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc &func, const std::string name = std::string());
    ~Thread();

    void start();
    void join();
private:
    bool _started;
    bool _joined;
    std::string _threadName;
    pid_t _tid;
    pthread_t _ptid;
    ThreadFunc _threadFunc;
    countDownLatch _count;
};


#endif