/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-07 02:30:21 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 05:15:45 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_SERVICE_SERVICE_H_
#define KBMS_SERVICE_SERVICE_H_

#include<iostream>

#include "event2/http.h"
#include "event2/event.h"
#include "event2/buffer.h"

#include <stdlib.h>
#include <stdio.h>

#include "http_context.h"
#include "network/io_thread.h"
#include "concurrency/thread_pool.h"

namespace kbms {

    class Service {
    public:
        Service(int32_t port, int32_t thread_num, uint64_t expiration);

        virtual ~Service();

        void Stop();

        bool Run();

        void RegisterHandler(const std::string &uri, HTTPRequestCallback callback);

        void RegisterDefaultHandler(HTTPRequestCallback callback);

        void RegisterTimeoutHandler(HTTPRequestCallback callback);

        bool Start();

    private:
        static void HttpGenericCallback(struct evhttp_request *request, void *arg);

        void CoreHandleRequest(struct evhttp_request *req);

        void Complete(const HttpContextPtr &ctx, const std::string &res_body);

        bool Listen();

    private:
        int32_t port_;
        uint64_t expiration_{0};
        CallbackMap callbacks_map_;
        //default callback function
        HTTPRequestCallback default_cb_;
        //timeout callback
        HTTPRequestCallback timeout_cb_;
        //io thread
        std::shared_ptr<IoThread> io_thread_;
        //worker thread pool
        std::shared_ptr<ThreadPool> thread_pool_;
    };

}
#endif // KBMS_SERVICE_SERVICE_H_
