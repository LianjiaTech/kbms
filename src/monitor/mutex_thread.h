/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-25 04:48:45 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:13:17 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KE_MICRO_SERVICE_SRC_NET_CONCURRENCY_MUTEX_THREAD_
#define KE_MICRO_SERVICE_SRC_NET_CONCURRENCY_MUTEX_THREAD_

#include <memory>
#include <mutex>
#include <chrono>

namespace kbms {

    class Guard {
    public:
        Guard(std::timed_mutex &value, int64_t timeout = 0) : mutex_(&value) {
            if (timeout == 0) {
                value.lock();
            } else if (timeout < 0) {
                if (!value.try_lock()) {
                    mutex_ = nullptr;
                }
            } else {
                if (!value.try_lock_for(std::chrono::milliseconds(timeout))) {
                    mutex_ = nullptr;
                }
            }
        }

        ~Guard() {
            if (mutex_) {
                mutex_->unlock();
            }
        }

        operator bool() const { return (mutex_ != nullptr); }

    private:
        //noncopyable
        Guard(const Guard &);

        void operator=(const Guard &);

        std::timed_mutex *mutex_;
    };

}
#endif // KE_MICRO_SERVICE_SRC_NET__CONCURRENCY_MUTEX_THREAD_ 
