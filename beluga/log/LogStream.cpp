// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "LogStream.h"
#include <algorithm>
#include <limits>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <cstdio>


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