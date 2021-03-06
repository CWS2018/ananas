#include "./logfile.h"

LogFile::LogFile(const std::string &filename)
    : _filename(filename),
      _file()
      //, _mutex()
{
    //rollfile()                                // 没有实现自动归档功能
    _file.open(_filename);
}

void LogFile::flush() {
    //MutexLock lock(_mutex)
    _file.flush();
}

void LogFile::append(const char *msg, int len) {
    //MutexLock lock(_mutex)
    _file.append(msg, len);
}