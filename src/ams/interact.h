/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-24 11:12:09 AM
 @Created By : dongzhixiong
 @Description: interact with registry center
 @Last modified: 2021-08-03 11:22:14 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_AMS_INTERACT_H_
#define KBMS_AMS_INTERACT_H_

#include <string>
#include <thread>
#include <atomic>
#include <vector>

namespace kbms {
    class Interact {
    public:
        typedef std::vector<std::string> VecStr;
    public:
        struct ServerStatus {
            std::string host;
            std::string port;
            std::string status;
            int state;
            int persist_failed_time;
            int when_last_failed;
            int failed_count;
        };

    public:
        Interact(const std::string &url, const std::string &host,
                 uint32_t port, const std::string &version,
                 const std::string &service_name, uint32_t timeout,
                 uint32_t renewal_secs, uint32_t duration_secs);

        virtual ~Interact();

        //put service status(STARTING,UP,DOWN)
        bool UpdateStatus(const std::string &ser_status) const;

        //get service instance  by service name
        virtual bool AcqInstance(const std::string &name, std::string &ser_instance) const;

        // interrupt interacting with registry
        virtual bool Interrupt() const;

        //start thread
        void Run();

        //stop thread
        void Stop();

    private:
        void DoUpdating();

        void StopUpdating();

        bool KeepActive() const;

        bool ServerDisc() const;

    private:
        class Impl;

        Impl *impl_;

        std::shared_ptr<std::thread> thread_;
        //flag of update interaction
        std::atomic<bool> flag_of_updating_is_on_;
        //update interval
        uint32_t updating_time_span_ = 2;
    };

}
#endif // KBMS_AMS_INTERACT_H_
