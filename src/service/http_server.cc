/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2020-01-08 05:17:31 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 05:22:28 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <signal.h>
#include <unistd.h>
#include <string>

#include "utils/utils.h"
#include "utils/logging.h"
#include "http_server.h"
#include "ams/interact.h"
#include "config/config_loader.h"

namespace kbms {

    class HttpServer::Impl {
    public:
        Impl(uint32_t port, RegistryInfo rti, ConfigInfo cfi) {
            InitInteractObj(port, rti);
            InitConfigObj(cfi);
        }

        ~Impl() {
            if (interact_) delete interact_;
            if (cfl_) delete cfl_;
            cfl_ = nullptr;
            interact_ = nullptr;
        }

        bool UpdateStatus(const std::string &value) {
            return interact_->UpdateStatus(value);
        }

        bool AcqInstance(const std::string &name, std::string &ser_instance) {
            return interact_->AcqInstance(name, ser_instance);
        }

        bool Interrupt() {
            return interact_->Interrupt();
        }

        void StartInteract() {
            interact_->Run();
        }

        void StartConfig() {
            cfl_->Start();
        }

        std::string GetConfig() {
            return cfl_->GetConfig();
        }

        void Stop() {
            //stop updating config
            cfl_->Stop();
            //stop interacting with registry
            Interrupt();
        }

    private:
        //init Interact object
        void InitInteractObj(uint32_t port, RegistryInfo rti) {
            if (!rti.flag_of_mmserver_on) return;
            std::string host;
            if (!GetHost(host)) return;
            interact_ = new Interact(rti.url, host,
                                     port, rti.version,
                                     rti.service_name,
                                     rti.timeout,
                                     rti.renewal_secs,
                                     rti.duration_sec);

        }

        //init config object
        void InitConfigObj(ConfigInfo cfi) {
            if (!cfi.flag_of_config_on) return;
            //creat ConfigLoader object
            cfl_ = new ConfigLoader(cfi.url);
            for (auto iter = cfi.headers.begin(); iter != cfi.headers.end(); iter++) {
                cfl_->SetReqHeader(iter->first, iter->second);
            }
        }

        //get local host ip
        bool GetHost(std::string &value) {
            char *ip_pointer = (char *) malloc(sizeof(char) * IP_SIZE);
            GetLocalIp(ip_pointer);
            if (!ip_pointer) {
                LOGGER_ERROR << "get local ip failed";
                return false;
            }
            value = ip_pointer;
            free(ip_pointer);
            ip_pointer = nullptr;
            return true;
        }

    private:
        Interact *interact_;
        ConfigLoader *cfl_;
    };

    HttpServer *HttpServer::instance_(nullptr);

    HttpServer::HttpServer() {
        instance_ = this;
    }

    HttpServer::~HttpServer() {
        if (service_) {
            delete service_;
        }

        if (instance_) {
            delete instance_;
        }

        instance_ = nullptr;
        service_ = nullptr;
        Stop();
    }

    HttpServer *HttpServer::Instance() {
        if (nullptr == instance_) {
            instance_ = new HttpServer();
        }
        return instance_;
    }

    bool HttpServer::Init(uint32_t port, uint32_t thread_num, uint64_t expiration, RegistryInfo rti, ConfigInfo cfi) {
        service_ = new Service(port, thread_num, expiration);
        flag_of_mmserver_on_ = rti.flag_of_mmserver_on;
        flag_of_config_on_ = cfi.flag_of_config_on;
        impl_ = new Impl(port, rti, cfi);
        if (service_ && impl_) return true;
        return false;
    }

    bool HttpServer::Init(uint32_t port, uint32_t thread_num, RegistryInfo rti, ConfigInfo cfi) {
        service_ = new Service(port, thread_num, 0);
        flag_of_mmserver_on_ = rti.flag_of_mmserver_on;
        flag_of_config_on_ = cfi.flag_of_config_on;
        impl_ = new Impl(port, rti, cfi);
        if (service_ && impl_) return true;
        return false;
    }

    bool HttpServer::Init(uint32_t port, uint32_t thread_num, uint64_t expiration) {
        flag_of_mmserver_on_ = false;
        flag_of_config_on_ = false;
        service_ = new Service(port, thread_num, expiration);
        if (service_) return true;
        return false;
    }

    void HttpServer::Stop() {
        const_cast<HttpServer::Impl *>(impl_)->Stop();
    }

    void HttpServer::StartServer() {

        if (flag_of_config_on_) this->StartConfig();

        auto ready_fun = [this]() {
            this->UpdateStatus("STARTING");
            //regist signal
            signal(SIGTERM, SignalHandler);
        };
        if (flag_of_mmserver_on_) ready_fun();

        // start net service
        service_->Start();

        auto finish_func = [this]() {
            this->UpdateStatus("UP");
            this->StartInteract();
        };
        if (flag_of_mmserver_on_) finish_func();


        while (true) {
            usleep(1);
        }
    }

    void HttpServer::RegisterFun(const std::string &uri, HTTPRequestCallback callback) {
        service_->RegisterHandler(uri, callback);
    }

    void HttpServer::RegisterDefaultFun(HTTPRequestCallback callback) {
        service_->RegisterDefaultHandler(callback);
    }

    void HttpServer::RegisterExpireFun(HTTPRequestCallback callback) {
        service_->RegisterTimeoutHandler(callback);
    }

    std::string HttpServer::GetConfig() const {
        const_cast<HttpServer::Impl *>(impl_)->GetConfig();
    }

    bool HttpServer::UpdateStatus(const std::string &value) const {
        return const_cast<HttpServer::Impl *>(impl_)->UpdateStatus(value);
    }

    void HttpServer::StartConfig() const {
        const_cast<HttpServer::Impl *>(impl_)->StartConfig();
    }

    void HttpServer::StartInteract() const {
        const_cast<HttpServer::Impl *>(impl_)->StartInteract();
    }

    bool HttpServer::Interrupt() const {
        return const_cast<HttpServer::Impl *>(impl_)->Interrupt();
    }

    bool HttpServer::AcqInstance(const std::string &name, std::string &ser_instance) const {
        return const_cast<HttpServer::Impl *>(impl_)->AcqInstance(name, ser_instance);
    }

    void HttpServer::SignalHandler(int signum) {
        instance_->Interrupt();
        //exit server
        exit(signum);
    }

}
