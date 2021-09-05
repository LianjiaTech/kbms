/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2020-01-08 04:58:46 PM
 @Created By : dongzhixiong
 @Description: External interface
 @Last modified: 2021-08-03 06:00:30 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_SERVICE_HTTP_SERVER_H_
#define KBMS_SERVICE_HTTP_SERVER_H_

#include <string>
#include <iostream>

#include "service.h"
#include "utils/formats.h"

namespace kbms {
    class HttpServer {
    public:
        virtual ~HttpServer();

        static HttpServer *Instance();

        bool Init(uint32_t port, uint32_t thread_num, RegistryInfo rti, ConfigInfo cfi);

        bool Init(uint32_t port, uint32_t thread_num, uint64_t expiration, RegistryInfo rti, ConfigInfo cfi);

        bool Init(uint32_t port, uint32_t thread_num, uint64_t expiration = 0);

        //start server
        void StartServer();

        //stop server
        void Stop();

        //register handler
        void RegisterFun(const std::string &uri, HTTPRequestCallback callback);

        void RegisterDefaultFun(HTTPRequestCallback callback);

        void RegisterExpireFun(HTTPRequestCallback callback);

        //get online config
        virtual std::string GetConfig() const;

        //update service status(UP,DOWN,STARTING)
        virtual bool UpdateStatus(const std::string &value) const;

        virtual bool AcqInstance(const std::string &name, std::string &ser_instance) const;

    private:
        virtual void StartConfig() const;

        virtual void StartInteract() const;

        //regist signal
        static void SignalHandler(int signum);

        //interrupt server
        virtual bool Interrupt() const;

        HttpServer();

    private:
        static HttpServer *instance_;
        //network module
        Service *service_;

        //other module
        class Impl;

        Impl *impl_;

        //whether start using config 
        bool flag_of_mmserver_on_;
        //whether start using micro_service
        bool flag_of_config_on_;
    };

}
#endif // KBMS_SERVICE_HTTP_SERVER_H_
