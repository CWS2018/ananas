#ifndef MUTEX_H_
#define MUTEX_H_

#include "./threadId.h"
#include "./noncopy.h"

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include <iostream>

class Mutex : noncopy
{
    /*
     * mutex的创建和销毁
     */
public:
    Mutex() : _ower(0) {
        pthread_mutex_init(&_mutex, NULL);
    }
    ~Mutex() {
        pthread_mutex_destroy(&_mutex);
    }

    void lock() {
        // 上锁
        pthread_mutex_lock(&_mutex);
        //_ower = threadId::tid();
    }
    void unlock() {
        // 解锁
        //assert(_ower == threadId::tid());
        _ower = 0;
        pthread_mutex_unlock(&_mutex);
    }

    pthread_mutex_t *mutex() {
        return &_mutex;
    }
private:
    pthread_mutex_t _mutex;
    pid_t _ower;
};

class MutexLock : noncopy
{
public:
    MutexLock(Mutex &mutex) : _mutex(mutex) {
        _mutex.lock();
    }
    ~MutexLock() {
        _mutex.unlock();
    }
private:
    Mutex &_mutex;
};

#endif