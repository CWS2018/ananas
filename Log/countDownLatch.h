#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_

#include "./noncopy.h"
#include "./mutex.h"
#include "./condition.h"

#include <atomic>

class countDownLatch : noncopy
{
public:
    explicit countDownLatch(int v) : _count(v), _mutex(), _cond(_mutex) {}

    void wait() {
        MutexLock lock(_mutex);
        while(_count > 0)
            _cond.wait();
    }

    void countDown() {
        MutexLock lock(_mutex);
        --_count;
        if(_count == 0) {
            _cond.notifyAll();
        }
    }

private:
    int _count;
    mutable Mutex _mutex;
    Cond _cond;
};


#endif