#include "./condition.h"

#include <string.h>

void Cond::waitForSeconds(int seconds) {

    struct timespec abstime;
    bzero(&abstime, sizeof(abstime));
    abstime.tv_nsec = 0;
    abstime.tv_sec = seconds;

    pthread_mutex_t m = _mutex.mutex();
    pthread_cond_timedwait(&_cond, &m, &abstime);
}