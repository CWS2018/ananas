#include "./asynclog.h"

#include <utility>
#include <new>

AsyncLog::AsyncLog(const std::string logfilename, int seconds)
    : _working(false),
      _logfilename(logfilename),
      _seconds(seconds),
      _curBuffer(new Buffer),
      _nextBuffer(new Buffer),
      _buffers(),
      _mutex(),
      _cond(_mutex),
      _thread(std::bind(&AsyncLog::back_end_threadFunc, this), "Log")
{
    _curBuffer->bzero();
    _nextBuffer->bzero();
    _buffers.reserve(32);
}

AsyncLog::~AsyncLog() {
    if(_working) {
        stop();
    }
}

void AsyncLog::append(const char *msg, int len) {
    // 多个线程往后端输出日志，需要加锁，防止争用
    MutexLock lock(_mutex);

    if(_curBuffer->avail() > len) {
        // 未写满一个buffer
        _curBuffer->append(msg, len);
    }
    else {
        // _curBuffer 无法写下一条日志
        _buffers.push_back(std::move(_curBuffer));

        if(_nextBuffer) {
            // 备用的buffer可用
            _curBuffer = std::move(_nextBuffer);
        }
        else {
            // 动态分配一个buffer
            _curBuffer.reset(new Buffer);
        }
        _curBuffer->append(msg, len);

        // 通知后端线程，可以将日志写入logfile
        _cond.notify();
    }
}

void AsyncLog::back_end_threadFunc() {
    /*
     * 后端接口， 将日志写入文件
     */

    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    newBuffer1->bzero();
    newBuffer2->bzero();
    buffersToWrite.reserve(32);
}