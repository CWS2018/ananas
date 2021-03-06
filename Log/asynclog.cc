#include "./asynclog.h"
#include "./logfile.h"

#include <assert.h>

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

    assert(_working == true);

    // 初始化，两块new buffer是为前端的two buffer准备
    // 尽最大努力确保前端的的日志有足够的buffer写入
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    newBuffer1->bzero();
    newBuffer2->bzero();
    buffersToWrite.reserve(32);

    // LogFile-Obj 用于将缓存里的日志写到磁盘
    LogFile output(_logfilename);

    while(_working) {
        /*
         * 每循环一次，如果有缓存就把它写到磁盘
         * 如果没有，每_seconds秒，就执行一次写盘
         */

        // 确保two newbuffer 存在且没有数据
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());        

        {
            // 临界区

            MutexLock lock(_mutex);
            if(_buffers.empty()) {
                // 不用使用while循环，因为要每_seconds刷新一次
                // 即使存在fake awake也无关要紧
                _cond.waitForSeconds(_seconds);
            }

            _buffers.push_back(std::move(_curBuffer));
            _curBuffer = std::move(newBuffer1);
            buffersToWrite.swap(_buffers);

            if(!_nextBuffer) {
                // _nextBuffer 已经给_curBuffer使用
                // 为_nextBuffer准备buffer
                _nextBuffer = std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());

        if(buffersToWrite.size() > 32) {
            // 日志太多了，为避免日志堆积，保留前32条日志
            buffersToWrite.erase(buffersToWrite.begin()+32, buffersToWrite.end());
        }

        for(auto &buffer : buffersToWrite) {
            output.append(buffer->data(), buffer->length());
        }
        if(buffersToWrite.size() > 32) {
            // erase() 不会改变物理空间
            // 重置
            buffersToWrite.resize(32);
        }

        if(!newBuffer1) {
            // newBuffer1已经被使用
            // 从buffersToWrite回收内存空间
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        if(!newBuffer2) {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();                                     
        output.flush();
    }
    output.flush();
}