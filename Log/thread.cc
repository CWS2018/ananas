#include "./thread.h"
#include "./threadId.h"

#include <assert.h>

typedef Thread::ThreadFunc ThreadFunc;

class ThreadData
{
public:
    explicit ThreadData(const ThreadFunc &func, pid_t *tid, Mutex *mutex, Cond *cond)
        : _func(func),
         _tid(tid),
         _mutex(mutex),
         _cond(cond)
    {
    }

    void run() {
        *_tid = threadId::tid();
        {
            // 通知线程，以获取线程id
            MutexLock lock(*_mutex);
            _cond->notify();
        }

        _tid = nullptr;
        _mutex = nullptr;
        _cond = nullptr;

        _func();
    }
private:
    ThreadFunc _func;
    pid_t *_tid;
    Mutex *_mutex;
    Cond *_cond;
};

void* startThread(void *obj) {
    ThreadData *data = static_cast<ThreadData*>(obj);
    data->run();
    delete data;
}

Thread::Thread(const ThreadFunc &func, const std::string name = std::string())
    : _started(false),
      _joined(false),
      _threadName(name),
      _tid(0),
      _ptid(0),
      _threadFunc(func),
      _mutex(),
      _cond(_mutex)
{
}

Thread::~Thread() {
    if(_started && !_joined) {
        pthread_detach(_ptid);
    }
}

void Thread::start() {
    if(!_started) {
        ThreadData *data = new ThreadData(_threadFunc, &_tid, &_mutex, &_cond);
        _started = true;
        if(pthread_create(&_ptid, NULL, startThread, data)) {
            // 创建线程失败
            _started = false;
            delete data;
        }
        else {
            MutexLock lock(_mutex);
            _cond.wait();
            assert(_tid > 0);
        }
    }
}

void Thread::join() {
    if(_started && !_joined) {
        pthread_join(_ptid, NULL);
    }
}

