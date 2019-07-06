//
// Created by zhsy on 19-6-8.
//

#include "CellTask.h"
#include "../net/Connection.h"

std::string CellTask::getTask(){
    return _msg;
}

void CellTask::respond(std::string result){
    _conn->writeBuffer(result);
}
