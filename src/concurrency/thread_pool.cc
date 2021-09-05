/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-13 07:44:06 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 05:20:54 PM
 @Modified By: dongzhixiong
************************************************************************/
#include "thread_pool.h"
#include "utils/logging.h"

namespace kbms {
    ThreadPool::ThreadPool(int thread_num) :
            thread_num_(thread_num),
            idle_count_(0),
            monitor_(&mutex_) {
    }

    ThreadPool::~ThreadPool() {

    }

    void ThreadPool::AddTask(TaskPtr task) {
        Guard g(mutex_);
        task_queue_.push_back(task);
        if (idle_count_ > 0) {
            monitor_.Notify();
        }
    }

// thread worker
    class ThreadPool::ThreadWorker {
    public:
        ThreadWorker(ThreadPool *tpool, const int thread_id) : tpool_(tpool), thread_id_(thread_id) {

        }

        void operator()() {
            Guard g(tpool_->mutex_);

            bool survival = true;

            while (survival) {
                survival = IsSurvival();
                while (survival && tpool_->task_queue_.empty()) {
                    tpool_->idle_count_++;
                    tpool_->monitor_.Wait();
                    survival = IsSurvival();
                    tpool_->idle_count_--;
                }
                TaskPtr task;
                if (survival) {
                    if (!tpool_->task_queue_.empty()) {
                        task = tpool_->task_queue_.front();
                        tpool_->task_queue_.pop_front();
                        //set task state
                        if (task->state_ == WAITING) {
                            task->state_ = task->get_timeout_flag() &&
                                           *(task->get_expire_time()) < std::chrono::steady_clock::now() ? TIMEDOUT
                                                                                                         : EXECUTING;
                        }
                    }
                }
                if (task) {
                    if (task->state_ == EXECUTING) {
                        // carrying out a task  and release the lock 
                        tpool_->mutex_.unlock();
                        try {
                            task->Run();
                        } catch (...) {
                            LOGGER_ERROR << "occur an unknown error";
                        }
                        // this thread can have priority acquire  lock
                        tpool_->mutex_.lock();
                    } else if (task->state_ == TIMEDOUT && task->get_timeout_flag()) {
                        tpool_->mutex_.unlock();
                        task->ExpireRun();
                        tpool_->mutex_.lock();
                    }
                }
            }

        }

    private:
        bool IsSurvival() const {
            return tpool_->state_ == RUN;
        }

    private:
        ThreadPool *tpool_;
        int thread_id_;
    };

    void ThreadPool::Stop() {
        Guard g(mutex_);
        state_ = STOP;
        //Wakes up all waiting threads
        monitor_.NotifyAll();
        for (int i = 0; i < m_threads.size(); ++i) {
            m_threads[i].join();
        }
    }

    void ThreadPool::Init() {
        m_threads = std::vector<std::thread>(thread_num_);
        this->state_ = RUN;
        for (int i = 0; i < m_threads.size(); ++i) {
            m_threads[i] = std::thread(ThreadPool::ThreadWorker(this, i));
            // detach thread
            m_threads[i].detach();
        }
    }

}
