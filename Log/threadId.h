#ifndef THREADID_H_
#define THREADID_H_

#include <sys/types.h>

namespace threadId
{
    pid_t gettid();

    pid_t tid();
}


#endif