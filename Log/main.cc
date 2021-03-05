#include <iostream>
#include <sys/time.h>

int main()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    std::cout << tv.tv_usec << std::endl;
}