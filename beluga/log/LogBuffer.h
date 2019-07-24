/**
 * @author Zhang Senyan
 * Date: 2019-06-20
 *
 * 日志 Buffer ： 日志的载体
 * 用户进程先将日志写入到Buffer中，
 * 再由后台日志进程定时定量将日志写入文件
 *
 */

#ifndef BELUGA_LOGBUFFER_H
#define BELUGA_LOGBUFFER_H

#include <cstddef>
#include <cstring>
#include <algorithm>


//数字最多占用32字节
#define kMaxNumericSize 32

const char digits[] = "9876543210123456789";

/**
 * 数字转字符串，可以在转换过程中写入buffer
 *
 * @tparam T   数字类型
 * @param buf  转换后字符串存放位置
 * @param value   数字值
 * @return  转换后字符串长度
 *
 * @refitem Shuo Chen (muduo)
 */
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

/**
 *
 * @tparam N : Buffer容量
 */
template <size_t N>
struct LogBuffer {
public:

    //构造函数
    LogBuffer():_begin(0),_end(0){

    }
    ~LogBuffer(){

    }

    /**
     * 一个使用完的LogBuffer在放入ObjectPool之前需要销毁
     * 所谓销毁就是将前后游标复位， 非真正意义的销毁
     */
    void destory(){
        _begin=0;
        _end=0;
    }

    //将指定字符串写入LogBuffer
    int append(const char *ch,int len){

        //判断buffer是否能够装得下该字符串
        if(N-_end<len)
            return -1;

        //写入字符串
        memcpy(_data+_end,ch,len);

        //调整游标
        _end+=len;
        return len;
    }

    /**
     * 将泛型类型数字写入LogBuffer
     * @tparam T
     * @param v 待写入数字
     * @return  >0 写入长度
     *          -1 写入失败
     */
    template <typename T>
    int append(T v){

        //检测LogBuffer空间是否足够
        if(N-_end<kMaxNumericSize)
            return -1;

        //value to string 并写入 buffer
        int len=convert(_data+_end,v);

        //调节游标
        _end+=len;
        return len;
    }

    /**
     * 将Double类型数字写入LogBuffer
     * @param v 待写入数字
     * @return  >0 写入长度
     *          -1 写入失败
     */
    int append(double v){

        //容量检测
        if(N-_end<kMaxNumericSize)
            return -1;

        //写入Buffer
        int len=snprintf(_data+_end, kMaxNumericSize, "%.12g", v);;
        _end+=len;
        return len;
    }


    int append(long double v){

        //容量检测
        if(N-_end<kMaxNumericSize)
            return -1;

        //写入Buffer
        int len=snprintf(_data+_end, kMaxNumericSize, "%.12Lg", v);;
        _end+=len;
        return len;
    }

    /**
     * 读取buffer中数据
     * @return 指向有效数据的首部指针
     */
    char* getData(){
        return _data+_begin;
    }

    /**
     *
     * @return buffer中有效数据的长度
     */
    size_t length(){
        return _end-_begin;
    }

public:

    /**
     * 指向对象池中下一个对象
     * 利用该指针可以将对象池中的对象串接起来
     */
    LogBuffer* _next;

private:

    //缓存数组
    char _data[N];

    //前后游标， _begin和_end之间属于有效数据
    int _begin;
    int _end;
};


#endif //BELUGA_LOGBUFFER_H
