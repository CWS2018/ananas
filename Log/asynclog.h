#ifndef ASYNCLOG_H_
#define ASYNCLOG_H_

class AsyncLog
{
public:
    AsyncLog();
    ~AsyncLog();

    void start();

    void append(const char *msg, int len);
private:
};


#endif