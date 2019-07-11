//
// Created by zhsy on 19-7-10.
//

#ifndef HCCSERVER_LOGBUFFER_H
#define HCCSERVER_LOGBUFFER_H

#include <cstddef>
#include <cstring>
#include <algorithm>
#define kMaxNumericSize 32

const char digits[] = "9876543210123456789";

// From muduo
template<typename T>
size_t convert(char* buf, T value)
{
    static const char* _zero = digits + 9;
    T i = value;
    char *p = buf;
    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = _zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}


template <size_t N>
struct LogBuffer {
public:
    LogBuffer():_begin(0),_end(0){

    }
    ~LogBuffer(){

    }
    void destory(){
        _begin=0;
        _end=0;
    }
    int append(const char *ch,int len){
        if(N-_end<len)
            return -1;
        memcpy(_data+_end,ch,len);
        _end+=len;
        return len;
    }

    template <typename T>
    int append(T v){
        if(N-_end<kMaxNumericSize)
            return -1;
        int len=convert(_data+_end,v);
        _end+=len;
        return len;
    }
    int append(double v){
        if(N-_end<kMaxNumericSize)
            return -1;
        int len=snprintf(_data+_end, kMaxNumericSize, "%.12g", v);;
        _end+=len;
        return len;
    }
    int append(long double v){
        if(N-_end<kMaxNumericSize)
            return -1;
        int len=snprintf(_data+_end, kMaxNumericSize, "%.12Lg", v);;
        _end+=len;
        return len;
    }
    char* getData(){
        return _data+_begin;
    }
    size_t length(){
        return _end-_begin;
    }
public:
    LogBuffer* _next;
private:
    char _data[N];
    int _begin;
    int _end;
};


#endif //HCCSERVER_LOGBUFFER_H
