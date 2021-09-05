/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-20 02:15:15 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:54:58 AM
 @Modified By: dongzhixiong
************************************************************************/
#include "io_thread.h"

namespace kbms {

    IoThread::IoThread() {
        evbase_ = event_base_new();
        //add event_base into notify
        event_notify_.reset(new EventMonitor(evbase_, std::bind(&IoThread::ExecuteTask, this)));
        event_notify_->Initialize();
    }

    IoThread::~IoThread() {

    }

    bool IoThread::Start() {
        thread_.reset(new std::thread(std::bind(&IoThread::Run, this)));
        //Run();
        return true;
    }

    void IoThread::Run() {
        event_base_dispatch(evbase_);
    }

    event_base *IoThread::GetEventBase() {
        return evbase_;
    }

    void IoThread::ExecuteTask() {
        //start task
        std::vector<Functor> functors;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            task_functors_.swap(functors);
        }

        for (size_t i = 0; i < functors.size(); ++i) {
            functors[i]();
            --pending_functor_count_;
        }

    }

    void IoThread::Submit(Functor &&func) {
        std::unique_lock<std::mutex> lock(mutex_);
        task_functors_.push_back(std::move(func));
        ++pending_functor_count_;
        event_notify_->EventNotify();
    }

}
