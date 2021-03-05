#include "./threadId.h"

#include <unistd.h>
#include <sys/syscall.h>

namespace threadId
{
    __thread pid_t t_threadId = 0;

    pid_t gettid() {
        /*
         * gettid() 是OS 的系统调用，用来获取线程ID
         * glibc 没有封装这个系统调用
         * 要调用这个syscall, 需要通过 syscall() 来调用
         */
        return static_cast<pid_t>((syscall(SYS_gettid)));
    }

    pid_t tid() {
        if(t_threadId == 0) {
            t_threadId = gettid();
        }
        return t_threadId;
    }
}