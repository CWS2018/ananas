#include "./log.h"
#include "./threadId.h"
#include "./logbuffer.h"
#include "./asynclog.h"

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

static pthread_once_t g_pthread_once = PTHREAD_ONCE_INIT; 

// 全局唯一一个 AsyncLog对象，多个线程共享
static AsyncLog g_asynclog;

void asynclog_init() {
    // 初始化后端线程
    g_asynclog.start();
}

void g_output(const char *data, int len) {
    /*
     * 从前端到后端的接口
     * 将一条完整的日志输出到后端
     */

    // 调用一次
    pthread_once(&g_pthread_once, asynclog_init);

    // 输出到后端
    g_asynclog.append(data, len);
}

void Log::Impl::formatInput() {
    /* 首先格式化输出一条日志中的日期、时间、线程号
     *           日期      时间     微妙   线程号
     * 格式如："2021-3-3 22:35:21.123456 999999"
     */
    
    struct timeval tv;
    time_t time;
    char str_time[32];
    bzero(str_time, 32);

    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm *log_tm = localtime(&time);
    strftime(str_time, 32, "%Y-%m-%d %H:%M:%S.", log_tm);

    // 输出日期、时间， 如："2021-3-3 22:35:21."
    _logstream << str_time;

    // 输出微妙
    _logstream << tv.tv_usec << " ";

    // 输出线程号
    pid_t id = threadId::tid();
    _logstream << id << " ";
}

Log::Impl::Impl(const char *filename, int line) 
    : _logstream(),
      _basename(filename),
      _line(line)
{
    formatInput();
}

Log::Log(const char *filename, int line) 
    : _impl(filename, line)
{
}

Log::~Log() {
    /*
     * 输出一条日志的最后几部分：源文件名，所在行的行号
     * 格式如：                    源文件名:行号
     *       ××××××××××××××.... ——test.cc:21
     * 调用callbackFunc将整条日志传输到后端
     */

    _impl._logstream << "——" << _impl._basename << ":" << _impl._line << "\n";

    const LogStream::Buffer& buf(stream().buffer());

    // 调用 g_output 将一条日志输出到后端
    g_output(buf.data(), buf.length());
}