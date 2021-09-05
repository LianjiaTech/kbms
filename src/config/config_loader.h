/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-20 10:49:56 AM
 @Created By : chenkaijiang001
 @Description:
 @Last modified: 2021-08-03 11:32:21 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_CONFIG_CONFIG_LOADER_H_
#define KBMS_CONFIG_CONFIG_LOADER_H_

#include <atomic>
#include <map>
#include <thread>
#include <memory>

#include "config.hpp"
#include "utils/http_curl.h"

namespace kbms {

    class ConfigLoader {
    public:
        ConfigLoader(const std::string &config_url);

        virtual ~ConfigLoader();

        // Get config string by version name
        // input:
        //  version
        // output:
        //  config
        std::string GetConfig(std::string version = "latest") const;

        // hands on updating config
        bool HandsOnUpdate();

        // set request header
        void SetReqHeader(const std::string &key, const std::string &value);

        // set a new config url
        void SetURL(const std::string &config_url);

        //start thread
        void Start();

        //stop thread
        void Stop();

    private:
        // Start updating
        void StartUpdating();

        // Stop Updating
        void StopUpdating();

        bool DoUpdating();

    private:
        Config *config_;

        std::string config_url_;
        // seconds
        uint32_t updating_time_span_;
        // seconds
        uint32_t time_of_waiting_for_using_;

        std::atomic<bool> flag_of_updating_is_on_;
        std::atomic<bool> flag_of_updating_status_;
        //request params
        HttpConnectInfo conn_info_;
        HttpCurl *http_curl_;
        // thread of execution
        std::shared_ptr<std::thread> thread_;
    };
}
#endif // KBMS_CONFIG_CONFIG_LOADER_H_
