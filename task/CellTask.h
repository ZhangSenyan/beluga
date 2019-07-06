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
    std::string getTask();
    void respond(std::string result);
private:
    std::string _msg;
    std::shared_ptr<Connection> _conn;
};


#endif //HCCSERVER_CELLTASK_H
