/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-25 04:48:45 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:47:15 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_MONITOR_MONITOR_THREAD_H_
#define KBMS_MONITOR_MONITOR_THREAD_H_

#include <chrono>
#include <errno.h>
#include <iostream>
#include <condition_variable>

#include "mutex_thread.h"

namespace kbms {
    class MonitorThread {
    public:
        explicit MonitorThread(std::timed_mutex *value);

        virtual ~MonitorThread();

        void Lock();

        void Unlock();

        int WaitForTimeRelative(const std::chrono::milliseconds &timeout);

        int WaitForTimeRelative(uint64_t timeout_ms) {
            return WaitForTimeRelative(std::chrono::milliseconds(timeout_ms));
        }

        int WaitForever();

        void Wait(const std::chrono::milliseconds &timeout);

        void Wait(uint64_t timeout_ms = 0ULL) { this->Wait(std::chrono::milliseconds(timeout_ms)); }

        //Wakes up one thread waiting 
        void Notify() {
            condition_variable_.notify_one();
        }

        //Wakes up all waiting threads 
        void NotifyAll() {
            condition_variable_.notify_all();
        }

    private:
        //noncopyable
        MonitorThread(const MonitorThread &);

        void operator=(const MonitorThread &);

        std::condition_variable_any condition_variable_;
        std::timed_mutex *mutex_;
    };

}
#endif // KBMS_MONITOR_MONITOR_THREAD_H_
