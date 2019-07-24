/**
 * @author Zhang Senyan
 * Date: 2019-06-19
 *
 * 日志文件写入
 */

#ifndef BELUGA_LOGFILE_H
#define BELUGA_LOGFILE_H

#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include "beluga/base/Util.h"



class LogFile {
public:

    //构造函数
    explicit LogFile(std::string logPath=getCWD()+"/test.log");
    ~LogFile();

    //设置Log文件路径
    void resetLogPath(std::string logPath);

    //写入文件
    size_t write(const char* logline, size_t len);

    //冲刷缓冲区
    void flush();

    //
    void append(const char* logline, const size_t len);
private:
    std::string _logPath;
    FILE* _fp;
};


#endif //BELUGA_LOGFILE_H
