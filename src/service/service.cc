/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-07 02:48:04 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 05:16:27 PM
 @Modified By: dongzhixiong
************************************************************************/
#include "service.h"
#include "utils/logging.h"

namespace kbms {

    Service::Service(int32_t port, int32_t thread_num, uint64_t expiration) : port_(port), expiration_(expiration) {
        io_thread_ = std::make_shared<IoThread>();
        thread_pool_ = std::shared_ptr<ThreadPool>(new ThreadPool(thread_num));
        thread_pool_->Init();
    }

    Service::~Service() {

    }

    void Service::HttpGenericCallback(struct evhttp_request *request, void *arg) {
        Service *hsrv = static_cast<Service *>(arg);
        hsrv->CoreHandleRequest(request);

    }

    void Service::CoreHandleRequest(struct evhttp_request *request) {

        HttpContextPtr hct(new HttpContext(request));
        hct->Initialize();

        std::string body = hct->BodyByString();
        std::string url = hct->Uri();

        auto task_fun = [url, hct, this]() {
            auto f = std::bind(&Service::Complete, this, hct, std::placeholders::_1);
            if (this->callbacks_map_.find(url) != this->callbacks_map_.end()) {
                this->callbacks_map_[url](hct, f);
            } else {
                if (default_cb_) {
                    default_cb_(hct, f);
                }
            }
        };
        auto expire_fun = [hct, this]() {
            auto f = std::bind(&Service::Complete, this, hct, std::placeholders::_1);
            if (timeout_cb_) {
                timeout_cb_(hct, f);
            }
        };
        //add task into queue
        TaskPtr task = std::shared_ptr<Task>(new Task(task_fun, expire_fun, expiration_));
        thread_pool_->AddTask(task);
    }

    void Service::RegisterHandler(const std::string &uri, HTTPRequestCallback callback) {
        callbacks_map_[uri] = callback;
    }

    void Service::RegisterDefaultHandler(HTTPRequestCallback callback) {
        default_cb_ = callback;
    }

    void Service::RegisterTimeoutHandler(HTTPRequestCallback callback) {
        timeout_cb_ = callback;
    }

    void Service::Complete(const HttpContextPtr &ctx, const std::string &res_body) {
        auto f = [ctx, res_body]() {
            struct evbuffer *evbuf = evbuffer_new();
            if (!evbuf) {
                LOGGER_ERROR << "create evbuffer failed!";
                return;
            }
            auto request = ctx->GetReq();
            evbuffer_add(evbuf, res_body.c_str(), res_body.size());
            evhttp_send_reply(request, HTTP_OK, "OK", evbuf);
            evbuffer_free(evbuf);
        };
        //sumbit task into io_thread
        io_thread_->Submit(f);
    }

    bool Service::Listen() {
        //init event base
        struct event_base *base = io_thread_->GetEventBase();
        if (!base) {
            LOGGER_ERROR << "create event_base failed!";
            return false;
        }
        //create http event
        struct evhttp *http = evhttp_new(base);
        if (!http) {
            LOGGER_ERROR << "create evhttp failed!";
            return false;
        }
        //bind port
        if (evhttp_bind_socket(http, "0.0.0.0", port_) != 0) {
            LOGGER_ERROR << "bind socket failed" << port_;
            return false;
        }
        //regist core handler
        evhttp_set_gencb(http, &Service::HttpGenericCallback, this);
        return true;
    }

    bool Service::Start() {
        Listen();
        io_thread_->Start();
        return true;

    }

}
