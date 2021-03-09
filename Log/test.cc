#include "./log.h"
#include "./thread.h"

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>
#include <vector>

void test() {
    std::cout << "****************** test *********************" << std::endl;
    LOG_INFO << (short)1;
    LOG_INFO << 2;
    LOG_INFO << (unsigned int) 3;
    LOG_INFO << (long) 4;
    LOG_INFO << (unsigned long) 5;
    LOG_INFO << (long long) 6;
    LOG_INFO << (unsigned long long) 7;

    LOG_INFO << 'a';
    LOG_INFO << "nwuking";
    LOG_INFO << std::string("China");
}

void test1() {
    sleep(1);
    for(int i = 0; i < 1000; ++i) {
        LOG_INFO << i;
    }
}

// 模拟多线程
void multiThread_test() {
    std::vector<std::shared_ptr<Thread>> ve;
    for(int i = 0; i < 5; ++i) {
        std::shared_ptr<Thread> th(new Thread(test1, "test"));
        ve.push_back(th);
    }
    for(int i = 0; i < 5; ++i) {
        ve[i]->start();
    }
    sleep(3);
}

int main()
{
    test();
    sleep(3);

    multiThread_test();
}