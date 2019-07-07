//
// Created by zhsy on 19-6-8.
//

#include "CellTask.h"
#include "../net/Connection.h"

CellTask::CellTask(const std::string& msg,std::shared_ptr<Connection> conn):_msg(msg),_conn(conn){

}
CellTask::~CellTask() {

}
std::string CellTask::getTask(){
    return _msg;
}

void CellTask::respond(std::string result){
    _conn->writeBuffer(result);
}
