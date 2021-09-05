/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-12 08:18:37 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 06:37:25 PM
 @Modified By: dongzhixiong
************************************************************************/
#include "task.h"

namespace kbms {

    Task::Task(TaskFun &&task_fun, TaskFun &&expire_fun, uint64_t expiration) {
        task_fun_ = std::move(task_fun);
        expire_fun_ = std::move(expire_fun);
        state_ = WAITING;
        flag_set_timeout_ = (expire_fun_ && expiration > 0) ? true : false;

        if (expiration != 0ULL) {
            expire_time_.reset(new std::chrono::steady_clock::time_point(
                    std::chrono::steady_clock::now() + std::chrono::milliseconds(expiration)));
        }

    }

    Task::Task(TaskFun &&task_fun) {
        task_fun_ = std::move(task_fun);
        state_ = WAITING;
    }

    Task::~Task() {

    }

    void Task::Run() {
        if (state_ == EXECUTING) {
            task_fun_();
            state_ = COMPLETE;
        }
    }

    void Task::ExpireRun() {
        if (state_ == TIMEDOUT) {
            expire_fun_();
            state_ = COMPLETE;
        }
    }
} 
