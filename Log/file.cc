#include "./file.h"

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
    size_t remain = len - n;

    while(remain > 0) {
        size_t m = fwrite(msg+n, remain, 1, _fp);
        if(m == 0) {
            int err = ferror(_fp);
            if(err) {
                fprintf(stderr, "File::append() failed!\n");
            }
            break;
        }
        n += m;
        remain = len - n;
    }
}

void File::reset(const std::string &filename) {
    this->close();
    this->open(filename);
}