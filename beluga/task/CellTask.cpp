//
// Created by zhsy on 19-6-8.
//

#include "CellTask.h"
#include "Connection.h"

CellTask::CellTask(std::string msg,std::shared_ptr<Connection> conn)
:_msg(std::move(msg)),_conn(std::move(conn)){

}
CellTask::~CellTask() {

}
std::string CellTask::getTask(){
    return _msg;
}

void CellTask::respond(std::string result){
    _conn->writeBuffer(std::move(result));
}
