#include "./log.h"
#include "./thread.h"

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>

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

int main()
{
    test();
    sleep(3);
}