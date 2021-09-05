/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-24 11:24:49 AM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 11:23:00 AM
 @Modified By: dongzhixiong
************************************************************************/
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <atomic>
#include <memory>
#include <map>
#include <vector>
#include <chrono>
#include <iostream>

#include "utils/http_curl.h"
#include "interact.h"
#include "utils/utils.h"
#include "utils/logging.h"
#include "args.h"

#define MAXNUM 2

namespace kbms {
    class Interact::Impl {
    public:
        typedef std::map<std::string, std::vector<ServerStatus> > ServerMap;
    public:
        Impl(const std::string &url, const std::string &host,
             uint32_t port, const std::string &version,
             const std::string &service_name, uint32_t timeout,
             uint32_t renewal_secs, uint32_t duration_secs) :
                url_(url),
                host_(host),
                port_(port),
                version_(version),
                service_name_(service_name),
                timeout_(timeout_),
                renewal_secs_(renewal_secs),
                duration_secs_(duration_secs),
                args_(new Args(host, std::to_string(port))) {
            //init ServerMap
            flag_num_ = 0;
            for (int i = 0; i < MAXNUM; i++) {
                std::shared_ptr<ServerMap> tmp = std::make_shared<ServerMap>();
                server_vec_.push_back(tmp);
            }

        }

        ~Impl() {
            if (args_) delete args_;
            args_ = nullptr;

        }

        // interrupt interacting with registry
        void GenInstance(std::string &instance_id) {
            VecStr vs_ins = {host_, args_->colon, std::to_string(port_), args_->minus_sign, version_};
            Join(vs_ins, instance_id);
        }

        bool Interrupt() {
            //to lower
            std::string service_name_lower;
            service_name_lower.resize(service_name_.size());
            std::transform(service_name_.begin(), service_name_.end(), service_name_lower.begin(), ::tolower);
            std::string instance_id;
            GenInstance(instance_id);

            VecStr del_vs = {url_, service_name_lower, instance_id};
            std::string url_del;
            Join(del_vs, args_->slanting_bar, url_del);

            VecStr put_vs = {url_, service_name_lower, instance_id, args_->down_tag};
            std::string url_put;
            Join(put_vs, args_->slanting_bar, url_put);

            HttpConnectInfo info;
            info.timeout = 1000;
            Response put_response, del_response;
            std::string request_body;

            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();

            request->Put(url_put, request_body, info, put_response);
            request->Del(url_del, info, del_response);
            return put_response.code == SC_SUCCESS && del_response.code == SC_SUCCESS;
        }

        //regist local service
        bool ReqRegisterCenter(const std::string &service_status) {
            std::string request_body;
            if (!GenRegistBody(service_status, request_body)) return false;
            VecStr vs = {url_, service_name_};
            std::string regist_url;
            Join(vs, args_->slanting_bar, regist_url);

            HttpConnectInfo info;
            info.timeout = timeout_;
            //set request header

            for (auto iter = args_->registry_header.begin(); iter != args_->registry_header.end(); iter++) {
                info.headers.insert(std::pair<std::string, std::string>(iter->first, iter->second));
            }

            Response response;
            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();

            request->Post(regist_url, request_body, info, response);
            return response.code == RC_SUCCESS;
        }

        //send heart beat
        bool SendHeartbeat() {
            std::string instance_id;
            GenInstance(instance_id);

            VecStr vs = {url_, service_name_, instance_id};
            std::string heart_url;
            Join(vs, args_->slanting_bar, heart_url);
            Response response;
            HttpConnectInfo info;
            info.timeout = timeout_;
            std::string request_body = "";

            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();
            request->Put(heart_url, request_body, info, response);
            return response.code == SC_SUCCESS;
        }

        //get all registry service instance
        bool ReqSerList() {
            HttpConnectInfo info;
            info.timeout = timeout_;

            for (auto iter = args_->ser_header.begin(); iter != args_->ser_header.end(); iter++) {
                info.headers.insert(std::pair<std::string, std::string>(iter->first, iter->second));
            }

            Response response;
            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();

            request->Get(url_, info, response);
            if (response.code != SC_SUCCESS) return false;
            if (!ParserData(response.body)) return false;
            return true;
        }

        bool AcqInstance(const std::string &name, std::string &ser_instance) {
            int num = GetFlagNum() % MAXNUM;
            auto iter = server_vec_[num]->find(name);
            if (iter == server_vec_[num]->end()) {
                LOGGER_ERROR << "server name is not exist:" << name;
                return false;
            }
            std::vector<ServerStatus> host_list;
            host_list = iter->second;
            if (host_list.size() < 1) {
                LOGGER_ERROR << "server  ip is not exist:" << name;
                return false;
            }
            ser_instance = RandSample(host_list);
            return true;

        }

    private:
        std::string RandSample(const std::vector<ServerStatus> host_list) {
            //ms
            struct timeb timeSeed;
            ftime(&timeSeed);
            srand(timeSeed.time * 1000 + timeSeed.millitm);
            int index = rand() % (host_list.size());
            return args_->GetUrl(host_list[index].host, host_list[index].port);
        }

        bool GenRegistBody(const std::string &service_status, std::string &request_body) {
            //to lower
            std::string service_name_lower;
            service_name_lower.resize(service_name_.size());
            std::transform(service_name_.begin(), service_name_.end(), service_name_lower.begin(), ::tolower);
            //registry body
            JsonDoc request_json;
            JsonDoc instance;
            std::string instance_id;
            GenInstance(instance_id);
            instance[args_->instance_id] = instance_id;
            instance[args_->app] = service_name_;
            instance[args_->ip_addr] = host_;

            instance[args_->status_page_url] = args_->GetUrl(args_->status_page_url_value);
            instance[args_->health_check_url] = args_->GetUrl(args_->health_check_url_value);
            instance[args_->home_page_url] = args_->GetUrl();

            instance[args_->secure_vip_address] = service_name_lower;
            instance[args_->vip_address] = service_name_lower;
            JsonDoc data_center_info;
            data_center_info[args_->at_class] = args_->at_class_value;
            data_center_info[args_->name] = args_->name_value;
            instance[args_->data_center_info] = data_center_info;
            instance[args_->host_name] = host_;
            instance[args_->sstatus] = service_status;
            JsonDoc lease_info;
            lease_info[args_->renewal_interval_insecs] = renewal_secs_;
            lease_info[args_->duration_insecs] = duration_secs_;
            instance[args_->lease_info] = lease_info;
            JsonDoc port;
            port[args_->dolar] = port_;
            port[args_->at_enabled] = args_->at_enabled_value;
            instance[args_->port_name] = port;
            request_json[args_->instance] = instance;

            return JsonToStr(request_json, request_body);
        }

        bool ParserData(const std::string &data_str) {
            //get flag_num_
            int num = (GetFlagNum() + 1) % MAXNUM;
            //num=1 or num=0
            if (num > 1) return false;
            server_vec_[num]->clear();
            JsonDoc data_json;
            JsonDoc application_json;
            if (!StrToJson(data_str, data_json)) return false;
            try {
                application_json = data_json[args_->application][args_->application];
                for (int idx = 0; idx < application_json.size(); idx++) {
                    std::vector<ServerStatus> server_list;
                    std::string service_name;
                    service_name = application_json[idx][args_->name].asString();
                    for (int idy = 0; idy < application_json[idx][args_->instance].size(); idy++) {
                        ServerStatus ss;
                        ss.host = application_json[idx][args_->instance][idy][args_->ip_addr].asString();
                        ss.port = application_json[idx][args_->instance][idy][args_->port_name][args_->dolar].asString();
                        ss.status = application_json[idx][args_->instance][idy][args_->sstatus].asString();
                        //only get service of up
                        if (ss.status == args_->valid_status) {
                            server_list.push_back(ss);
                        }
                    }
                    if (server_list.empty()) continue;
                    server_vec_[num]->insert(std::make_pair(service_name, server_list));
                }
            } catch (...) {
                LOGGER_ERROR << "fail to parser data";
                return false;
            }
            //update flag_num_ 
            flag_num_.fetch_add(1, std::memory_order_relaxed);
            return true;
        }

        int GetFlagNum() {
            return flag_num_.load(std::memory_order_relaxed);
        }

    private:
        //registry center url
        std::string url_;
        //local ip
        std::string host_;
        //service port
        uint32_t port_;
        //service  version
        std::string version_;
        //service name
        std::string service_name_;

        //request timeout
        uint32_t timeout_;

        //renewal Interval In Secs
        uint32_t renewal_secs_;
        //duration secs
        uint32_t duration_secs_;

        //save service list
        std::vector<std::shared_ptr<ServerMap> > server_vec_;
        std::atomic<int> flag_num_;

        //args
        Args *args_;

    };

    Interact::Interact(const std::string &url, const std::string &host,
                       uint32_t port, const std::string &version,
                       const std::string &service_name, uint32_t timeout,
                       uint32_t renewal_secs, uint32_t duration_secs) :
            impl_(new Interact::Impl(url, host, port, version, service_name, timeout, renewal_secs, duration_secs)) {
    }

    Interact::~Interact() {
        if (impl_) {
            delete impl_;
        }
        impl_ = nullptr;
        Stop();
    }

    bool Interact::UpdateStatus(const std::string &ser_status) const {
        return const_cast<Interact::Impl *>(impl_)->ReqRegisterCenter(ser_status);
    }

    bool Interact::KeepActive() const {
        return const_cast<Interact::Impl *>(impl_)->SendHeartbeat();
    }

    bool Interact::ServerDisc() const {
        return const_cast<Interact::Impl *>(impl_)->ReqSerList();
    }

    bool Interact::AcqInstance(const std::string &name, std::string &ser_instance) const {
        return const_cast<Interact::Impl *>(impl_)->AcqInstance(name, ser_instance);
    }

    bool Interact::Interrupt() const {
        return const_cast<Interact::Impl *>(impl_)->Interrupt();
    }

    void Interact::DoUpdating() {
        flag_of_updating_is_on_ = true;
        while (flag_of_updating_is_on_) {
            if (!KeepActive()) {
                LOGGER_ERROR << "fail to send heart";
            }

            if (!ServerDisc()) {
                LOGGER_ERROR << "fail to get list of registry service ";
            }
            //LOGGER_INFO << "this thread  that send heart beat and get service list  is alive ";
            std::chrono::seconds waiting_seconds(updating_time_span_);
            std::this_thread::sleep_for(waiting_seconds);
        }

    }

    //stop update
    void Interact::StopUpdating() {
        flag_of_updating_is_on_.store(false);
    }

    void Interact::Run() {
        //start thread
        thread_.reset(new std::thread(std::bind(&Interact::DoUpdating, this)));
    }

    void Interact::Stop() {
        StopUpdating();
        if (thread_ && thread_->joinable()) {
            thread_->join();
            thread_.reset();
        }
    }

}
