// @Author Lin Ya
// @Email xxbbb@vip.qq.com

#ifndef HCCSERVER_LOGSTREAM_H
#define HCCSERVER_LOGSTREAM_H
#include "noncopyable.h"
#include <cassert>
#include <cstring>
#include <string>
#include "AsynLogging.h"

//class AsynLogging;
//const int kLargeBuffer = 2000 * 1000;

class LogStream : noncopyable
{
public:
    static LogStream& instance(){
        static LogStream logStream;
        return logStream;
    }
    void resetPath(std::string path){
        asynLogging.setLogPath(std::move(path));
    }
    LogStream& operator<<(bool v)
    {
        asynLogging.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const void*);

    LogStream& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(char v)
    {
        asynLogging.append(&v, 1);
        return *this;
    }

    LogStream& operator<<(const char* str)
    {
        if (str)
            asynLogging.append(str, strlen(str));
        else
            asynLogging.append("(null)", 6);
        return *this;
    }

    LogStream& operator<<(const unsigned char* str)
    {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const std::string& v)
    {
        asynLogging.append(v.c_str(), v.size());
        return *this;
    }
    void append(const char* data, int len) { asynLogging.append(data, len); }
private:
    AsynLogging asynLogging;
    LogStream():asynLogging(){
        asynLogging.startLoop();
    }
};

#define LOG LogStream::instance()


#endif