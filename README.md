# ananas
A simple async log


这是一个简单的异步日志，学习了陈硕老师的muduo中的日志部分，不过没有陈硕老师写的高深。    
因为是用来学习练手，没有实现自动归档功能，把异步日志大体上实现了。       

************      

异步日志分为前端和后端两部分：         
    * 前端：由多个线程共享，通过接口`AsyncLog::append()`将各自的一条日志输出到缓存区里，利用`AsyncLog::_mutex`      
    保证多线程的同步。前端不执行IO，它会在缓存区`AsyncLog::_curBuffer`写不下时，将这一大块日志传输到后端，并通过      
    条件变量`AsyncLog::_cond`唤醒后端线程来将日志`flush`到磁盘。                     
    * 后端：仅有一个线程占有，该线程在构建对象`AsyncLog`时通过单例`pthread_once()`创建并初始化一次，后端线程会等       
    待前端的唤醒、或者在等待一定的时间后，会执行IO操作，将日志写到磁盘里。         

**********************          

本[^async log]主要有六个文件构成：          
    * `log.h`：是用户接口。通过 `LOG_INFO << "nwuking";`，会自动构造出一条完整的日志，当`LOG_INFO`生命周期结束       
    时便将整条日志写到前端的缓存区里。                 
    * `logstream.h`：由`log.h`使用，是`<<`的重定向。                    
    * `logbuffer.h`：定义了供前端、后端使用的缓存区。                                
    * `asynclog.h`：是整个`async log`最核心的部分，定义了两个接口函数，前端接口：`AsyncLog::append()`和后端接        
    口：`AsyncLog::back_end_threadFunc()`。前端线程通过`AsyncLog::append()`将日志写入缓存区，后端线程通过           
    `AsyncLog::back_end_threadFunc()`将一整块日志写入磁盘。                          
    * `logfile.h`：由`AsyncLog::back_end_threadFunc()`使用。                  
    * `file.h`：是 `FILE *_fp`的封装，打开一个文件流，并执行实际的IO，又`logfile.h`使用。         

*********************            

双缓冲区技术：             
    实际上是4个`Buffer`。                               
    * 前端有两个`_curBuffer`和`_nextBuffer`，前端产生的日志会写入到`_curBuffer`中，当`_curBuffer`写满的时候         
    或者写不下一条日志的时候，就会通过`std::move()`窃取`_nextBuffer`的内存，或者创建一个新的`Buffer`。                                   
    * 后端有两个`newBuffer1`和`newBuffer2`，这两个`Buffer`是前端`_curBuffer`、`_nextBuffer`的备胎。      


感悟心得：                          
    * 学会了如何重复回收利用内存，对移动语意有了进一步的认识，`std::move()`的使用，避免了缓冲区的不断构造、销毁，重         
    复利用了内存空间，配合智能指针的`std::shared_ptr`、`std::unique_ptr`的使用，进一步避免内存泄露，简直                 
    perfect。                           
    * 编程技巧，陈硕老师的muduo中的`^convert(T v)`见`logstream.cc`的巧妙，一个将数值转换成字符串的函数，大佬           
    就是大佬，一个简单的函数都甩我几条街。             
    * RTTI的使用，刚开始没有看到`Log::~Log()`时，想不明白`LOG << "nwuking"`怎么就将日志写到文件里了。使用RTTI           
    技巧，再配合一个`{}`作用域，后顾无忧～～～
