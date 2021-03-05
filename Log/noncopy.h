#ifndef NONCOPY_H_
#define NONCOPY_H_

class noncopy
{
public:
    noncopy(const noncopy&) = delete;
    void operator=(const noncopy&) = delete;
protected:
    noncopy() = default;
    ~noncopy() = default;
};

#endif