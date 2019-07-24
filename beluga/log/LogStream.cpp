/**
 * @author Zhang Senyan
 * Date: 2019-06-27
 *
 * 日志接口
 * @refitem Shuo Chen(muduo)
 */


#include <algorithm>
#include <limits>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <cstdio>

#include "LogStream.h"


/**
 * 重载 operator<<
 * 转调用 asynLogging.append(v);
 */
LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
    asynLogging.append(v);
    return *this;
}

LogStream& LogStream::operator<<(long double v)
{
    asynLogging.append(v);
    return *this;
}