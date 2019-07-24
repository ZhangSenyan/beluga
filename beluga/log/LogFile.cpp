/**
 * @author Zhang Senyan
 * Date: 2019-06-19
 *
 * 日志文件写入
 */

#include "LogFile.h"


/**
 * 构造函数
 * @param logPath ： log文件路径
 */
LogFile::LogFile(std::string logPath):_logPath(logPath),_fp(fopen(logPath.c_str(), "ae")){

}

LogFile::~LogFile(){

}

/**
 * 更改log文件路径
 * @param logPath ： 更改后的log文件路径
 */
void LogFile::resetLogPath(std::string logPath){

    //将原来日志写入磁盘
    flush();

    //关闭原来的日志文件
    fclose(_fp);


    _logPath=logPath;
    std::cout<<logPath<<std::endl;


    //打开新的日志文件
    if(_fp=fopen(logPath.c_str(), "ae")){
        std::cout<<"Reset Path:"<<logPath<<std::endl;
    }
    else{
        perror("Reset Path");
        exit(-1);
    }
}

/**
 * 写入日志文件
 * @param logline   待写入日志内容
 * @param len   待写入日志长度
 * @return  -1 写入错误
 */
size_t LogFile::write(const char* logline, size_t len)
{
    return fwrite_unlocked(logline, 1, len, _fp);
}

//将缓存中的日志写入磁盘
void LogFile::flush()
{
    fflush(_fp);
}

/**
 * 写入日志文件
 * @param logline   待写入日志内容
 * @param len   待写入日志长度
 *
 */
void LogFile::append(const char* logline, const size_t len)
{

    size_t n = write(logline, len);
    size_t remain = len - n;
    while (remain > 0)
    {
        size_t x = write(logline + n, remain);
        if (x == 0)
        {
            int err = ferror(_fp);
            if (err)
                fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n += x;
        remain = len - n;
    }
}