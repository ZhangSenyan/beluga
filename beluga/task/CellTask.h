/**
 * @author Zhang Senyan
 * Date: 2019-05-03
 *
 * CellTask : 基本任务单元
 */

#ifndef BELUGA_CELLTASK_H
#define BELUGA_CELLTASK_H

#include <string>
#include <memory>

class Connection;

class CellTask {
public:

    /**
     * 构造的函数
     * @param msg  收到的消息
     * @param conn   消息对应的连接
     */
    CellTask(std::string msg,std::shared_ptr<Connection> conn);
    ~CellTask();

    //获取任务消息
    const std::string getTaskString() const;

    //回复消息
    void respond(std::string result);

    std::shared_ptr<Connection> getConn();
    std::string getConnAddress()const;
private:

    std::string _msg;

    std::shared_ptr<Connection> _conn;

    //收到消息的时间
    time_t _time;
};


#endif //BELUGA_CELLTASK_H
