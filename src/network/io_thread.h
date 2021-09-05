/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-20 11:57:22 AM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:53:19 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_NETWORK_IO_THREAD_H_
#define KBMS_NETWORK_IO_THREAD_H_

#include <memory>
#include <string>
#include <functional>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include <utility>
#include <future>

#include "monitor/event_monitor.h"

namespace kbms {

    class IoThread {
    public:
        typedef std::function<void()> Functor;
    public:
        IoThread();

        virtual ~IoThread();

        void Submit(Functor &&func);

        //get event_base
        event_base *GetEventBase();

        bool Start();

    private:
        void ExecuteTask();

        void Run();

    private:
        std::mutex mutex_;
        std::vector<Functor> task_functors_;
        std::atomic<int> pending_functor_count_;
        std::mutex m_conditional_mutex_;
        std::condition_variable m_conditional_lock_;
        bool active_ = true;
        // notify listen thread
        std::shared_ptr<EventMonitor> event_notify_;
        // event_base
        struct event_base *evbase_;
        //handler thread
        std::shared_ptr<std::thread> thread_;
    };

}
#endif // KBMS_NETWORK_IO_THREAD_H_

