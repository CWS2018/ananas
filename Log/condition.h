#ifndef CONDITION_H_
#define CONDITION_H_

#include "./noncopy.h"
#include "./mutex.h"

class Cond : noncopy
{
public:
    Cond(Mutex &mutex) : _mutex(mutex) {
        pthread_cond_init(&_cond, NULL);
    }
    ~Cond() {
        pthread_cond_destroy(&_cond);
    }

    void notify() {
        pthread_cond_signal(&_cond);
    }

    void notifyAll() {
        pthread_cond_broadcast(&_cond);
    }

    void wait() {
        pthread_mutex_t m = _mutex.mutex();
        pthread_cond_wait(&_cond, &m);
    }
private:
    pthread_cond_t _cond;
    Mutex &_mutex;
};

#endif