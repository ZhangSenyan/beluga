//
// Created by zhsy on 19-6-8.
//

#include "CellTask.h"
#include "beluga/net/Connection.h"

CellTask::CellTask(std::string msg,std::shared_ptr<Connection> conn)
:_msg(std::move(msg)),_conn(std::move(conn)),_time(0){
//_time(0) 这里以创建任务的时间近似表示接收任务的时间
}
CellTask::~CellTask() {

}
const std::string CellTask::getTaskString() const{
    return _msg;
}

void CellTask::respond(std::string result){
    _conn->writeBuffer(std::move(result));
}
std::shared_ptr<Connection> CellTask::getConn(){
    return _conn;
}
std::string CellTask::getConnAddress() const{
    return _conn->getIP()+":"+std::to_string(_conn->getPort());
}