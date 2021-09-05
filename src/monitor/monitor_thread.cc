/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-25 04:48:45 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:47:38 AM
 @Modified By: dongzhixiong
************************************************************************/
#include <assert.h>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>

#include "monitor_thread.h"
#include "utils/logging.h"

namespace kbms {

    MonitorThread::MonitorThread(std::timed_mutex *value) : mutex_(value), condition_variable_() {

    }

    MonitorThread::~MonitorThread() {
        mutex_ = nullptr;
    }


    void MonitorThread::Lock() {
        mutex_->lock();
    }

    void MonitorThread::Unlock() {
        mutex_->unlock();

    }

    void MonitorThread::Wait(const std::chrono::milliseconds &timeout) {
        int label = WaitForTimeRelative(timeout);
        if (label == ETIMEDOUT) {
            LOGGER_ERROR << "set timeout failed";
        } else if (label != 0) {
            LOGGER_ERROR << "set timeout exception";
        }
    }

    int MonitorThread::WaitForTimeRelative(const std::chrono::milliseconds &timeout) {
        if (timeout.count() == 0) {
            return WaitForever();
        }

        assert(mutex_);
        std::unique_lock<std::timed_mutex> lock(*mutex_, std::adopt_lock);
        bool timedout = (condition_variable_.wait_for(lock, timeout) == std::cv_status::timeout);
        lock.release();
        return (timedout ? ETIMEDOUT : 0);
    }

    int MonitorThread::WaitForever() {
        assert(mutex_);
        std::unique_lock<std::timed_mutex> lock(*mutex_, std::adopt_lock);
        condition_variable_.wait(lock);
        lock.release();
        return 0;
    }

}
