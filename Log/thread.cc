#include "./thread.h"
#include "./threadId.h"

#include <assert.h>
#include <iostream>

typedef Thread::ThreadFunc ThreadFunc;

class ThreadData
{
public:
    explicit ThreadData(const ThreadFunc &func, pid_t *tid, countDownLatch *count)
        : _func(func),
         _tid(tid),
        _count(count)
    {
    }

    void run() {
        *_tid = threadId::tid();
        _tid = nullptr;
        _count->countDown();
        _count = NULL;
        _func();
    }
private:
    ThreadFunc _func;
    pid_t *_tid;
    countDownLatch *_count;
};

void* startThread(void *obj) {
    ThreadData *data = static_cast<ThreadData*>(obj);
    data->run();
    delete data;
}

Thread::Thread(const ThreadFunc &func, const std::string name)
    : _started(false),
      _joined(false),
      _threadName(name),
      _tid(0),
      _ptid(0),
      _threadFunc(func),
      _count(1)
{
}

Thread::~Thread() {
    if(_started && !_joined) {
        pthread_detach(_ptid);
    }
}

void Thread::start() {
    if(!_started) {
        ThreadData *data = new ThreadData(_threadFunc, &_tid, &_count);
        _started = true;
        if(pthread_create(&_ptid, NULL, startThread, data)) {
            // 创建线程失败
            _started = false;
            delete data;
        }
        else {
           _count.wait();
            assert(_tid > 0);
            //sleep(10);
        }
    }
}

void Thread::join() {
    if(_started && !_joined) {
        pthread_join(_ptid, NULL);
    }
}

