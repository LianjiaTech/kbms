/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-13 07:44:06 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:27:41 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_CONCURRENCY_THREAD_POOL_H_
#define KBMS_CONCURRENCY_THREAD_POOL_H_

#include <deque>
#include <vector>

#include "task.h"
#include "monitor/monitor_thread.h"

namespace kbms {
    typedef std::deque<TaskPtr> TaskQueue;

    class ThreadPool {
    public:
        enum TPOOL_STATE {
            UNKNOWING, START, RUN, STOP
        };
        typedef std::function<void(const std::string &response_data)> TimeoutCallback;
    public:
        explicit ThreadPool(int thread_num);

        virtual ~ThreadPool();

        ThreadPool(ThreadPool &&) = delete;

        ThreadPool &operator=(const ThreadPool &) = delete;

        ThreadPool &operator=(ThreadPool &&) = delete;

        //add task into thread pool
        void AddTask(TaskPtr task);

        void Init();

        void Stop();

    private:

        TimeoutCallback timeout_cb_;
        TaskQueue task_queue_;
        // Queue synchronization mechanism lock and condition variable
        //Mutex mutex_;
        std::timed_mutex mutex_;
        MonitorThread monitor_;
        //thread pool state
        TPOOL_STATE state_;

        class ThreadWorker;

        //idle thread num
        size_t idle_count_;
        //work thead
        std::vector<std::thread> m_threads;
        //thread num
        int thread_num_;
    };

}
#endif // KBMS_CONCURRENCY_THREAD_POOL_H_
