#include "./file.h"

#include <iostream>

void File::flush() {
    fflush(_fp);
}

void File::open(const std::string &filename) {
    _fp = fopen(filename.c_str(), "ae");
    // 在_fp打开文件流前，自定义缓存
    setbuffer(_fp, _buf, sizeof(_buf));
}

void File::append(const char *msg, int len) {
    size_t n = fwrite(msg, len, 1, _fp);
}

void File::reset(const std::string &filename) {
    this->close();
    this->open(filename);
}