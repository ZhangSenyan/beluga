/**
 * @author Zhang Senyan
 * Date: 2019-05-03
 *
 * CellTask : 基本任务单元
 */

#include "CellTask.h"
#include "beluga/net/Connection.h"

/**
 * 构造函数
 * _time(0) 这里以创建任务的时间近似表示接收任务的时间
 */
CellTask::CellTask(std::string msg,std::shared_ptr<Connection> conn):
                _msg(std::move(msg)),
                _conn(std::move(conn)),
                _time(0){

}
CellTask::~CellTask() {

}


const std::string CellTask::getTaskString() const{
    return _msg;
}

/**
 * 回复消息
 * 将结果写入发送缓冲区
 * @param result
 */
void CellTask::respond(std::string result){
    _conn->writeBuffer(std::move(result));
}

/**
 * 获取任务所属的连接
 * @return  Connection
 */
std::shared_ptr<Connection> CellTask::getConn(){
    return _conn;
}

/**
 * 获取任务所属的连接的地址
 * @return  Connection Address
 */
std::string CellTask::getConnAddress() const{
    return _conn->getIP()+":"+std::to_string(_conn->getPort());
}