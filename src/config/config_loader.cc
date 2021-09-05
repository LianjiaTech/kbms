/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-20 10:49:56 AM
 @Created By : chenkaijiang001
 @Description:
 @Last modified: 2021-08-03 12:13:32 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <utility>
#include <thread>
#include <chrono>

#include "utils/logging.h"
#include "config_loader.h"
#include "ams/args.h"

namespace kbms {

    ConfigLoader::ConfigLoader(const std::string &config_url) :
            updating_time_span_(10),
            time_of_waiting_for_using_(3),
            config_(nullptr) {
        config_url_ = config_url;
        flag_of_updating_is_on_ = false;
        http_curl_ = new HttpCurl();
        if (!http_curl_) {
            LOGGER_INFO << "init HttpCurl fail";
        }
    }

    ConfigLoader::~ConfigLoader() {
        if (config_) {
            delete config_;
        }
        config_ = nullptr;

        if (http_curl_) {
            delete http_curl_;
        }
        http_curl_ = nullptr;
    }


    std::string ConfigLoader::GetConfig(std::string version) const {
        if (nullptr == config_) return "";
        return config_->data_;
    }

    void ConfigLoader::Start() {
        //start thread
        thread_.reset(new std::thread(std::bind(&ConfigLoader::StartUpdating, this)));
    }

    void ConfigLoader::Stop() {
        StopUpdating();
        if (thread_ && thread_->joinable()) {
            thread_->join();
            thread_.reset();
        }

    }

    void ConfigLoader::SetURL(const std::string &config_url) {
        config_url_ = config_url;
    }

    void ConfigLoader::SetReqHeader(const std::string &key, const std::string &value) {
        conn_info_.headers.insert(std::pair<std::string, std::string>(key, value));
    }

    void ConfigLoader::StartUpdating() {
        flag_of_updating_is_on_ = true;
        while (flag_of_updating_is_on_) {
            flag_of_updating_status_.store(true, std::memory_order_relaxed);
            if (!DoUpdating()) {
                LOGGER_ERROR << "fail to update config";
            }
            flag_of_updating_status_.store(false, std::memory_order_relaxed);
            //LOGGER_INFO << "this thread that update config is alive";
            std::chrono::seconds waiting_seconds(updating_time_span_);
            std::this_thread::sleep_for(waiting_seconds);
        }
    }

    bool ConfigLoader::HandsOnUpdate() {
        // it will be fail if it is updating now.
        if (!flag_of_updating_status_.load(std::memory_order_relaxed)) {
            LOGGER_ERROR << "it is updating now";
            return false;
        }
        return DoUpdating();
    }

    void ConfigLoader::StopUpdating() {
        flag_of_updating_is_on_ = false;
    }

    bool ConfigLoader::DoUpdating() {
        Response response;
        http_curl_->Get(config_url_, conn_info_, response);
        if (response.code != SC_SUCCESS || response.body.size() == 0) {
            return false;
        }
        if (nullptr == config_ || (response.body != config_->data_)) {
            // update from remote location
            // parse it;

            Config *config_new = new Config;
            config_new->data_ = response.body;

            std::swap(config_new, config_);
            // waiting for it;
            std::chrono::seconds waiting_seconds(time_of_waiting_for_using_);
            std::this_thread::sleep_for(waiting_seconds);
            if (config_new) {
                delete config_new;
            }
        }

        return true;
    }

} 
