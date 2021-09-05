/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-12 08:13:05 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 06:36:22 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_CONCURRENCY_TASK_H_
#define KBMS_CONCURRENCY_TASK_H_

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <thread>

namespace kbms {

    enum TASK_STATE {
        WAITING, EXECUTING, TIMEDOUT, COMPLETE
    };

    class Task {
    public:
        typedef std::function<void()> TaskFun;
    public:
        Task(TaskFun &&task_fun, TaskFun &&expire_fun, uint64_t expiration = 0ULL);

        explicit Task(TaskFun &&task_fun);

        virtual ~Task();

        void Run();

        void ExpireRun();

        const std::unique_ptr<std::chrono::steady_clock::time_point> &get_expire_time() const { return expire_time_; }

        bool get_timeout_flag() const { return flag_set_timeout_; }

    public:
        TASK_STATE state_;
        TaskFun task_fun_;
        TaskFun expire_fun_;
        bool flag_set_timeout_{false};
        std::unique_ptr<std::chrono::steady_clock::time_point> expire_time_;
    };

    typedef std::shared_ptr<Task> TaskPtr;
}
#endif // KBMS_CONCURRENCY_TASK_H_ 
