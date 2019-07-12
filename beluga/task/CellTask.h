//
// Created by zhsy on 19-6-8.
//

#ifndef HCCSERVER_CELLTASK_H
#define HCCSERVER_CELLTASK_H

#include <string>
#include <memory>

class Connection;

class CellTask {
public:
    CellTask(std::string msg,std::shared_ptr<Connection> conn);
    ~CellTask();
    const std::string getTaskString() const;
    void respond(std::string result);
    std::shared_ptr<Connection> getConn();
    std::string getConnAddress()const;
private:
    std::string _msg;
    std::shared_ptr<Connection> _conn;
    time_t _time;
};


#endif //HCCSERVER_CELLTASK_H
