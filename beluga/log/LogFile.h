//
// Created by zhsy on 19-7-10.
//

#ifndef HCCSERVER_LOGFILE_H
#define HCCSERVER_LOGFILE_H

#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include "beluga/base/Util.h"
class LogFile {
public:
    explicit LogFile(std::string logPath=getCWD()+"/test.log"):_logPath(logPath),_fp(fopen(logPath.c_str(), "ae")){
        std::cout<<"LogFile"<<std::endl;
    }
    ~LogFile(){

    }
    void resetLogPath(std::string logPath){
        flush();
        fclose(_fp);
        _logPath=logPath;
        std::cout<<logPath<<std::endl;
        _fp=fopen(logPath.c_str(), "ae");
    }
    size_t write(const char* logline, size_t len)
    {


        return fwrite_unlocked(logline, 1, len, _fp);
    }
    void flush()
    {
        fflush(_fp);
    }
    void append(const char* logline, const size_t len)
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
private:
    std::string _logPath;
    FILE* _fp;
};


#endif //HCCSERVER_LOGFILE_H
