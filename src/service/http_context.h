/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-07 03:31:50 PM
 @Created By : dongzhixiong
 @Description: parser http context
 @Last modified: 2021-08-03 11:58:40 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_SERVICE_HTTP_CONTEXT_H_
#define KBMS_SERVICE_HTTP_CONTEXT_H_

#include <string>
#include <map>
#include <memory>
#include <functional>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/event_compat.h>
#include <event2/dns.h>
#include <event2/dns_compat.h>
#include <event2/dns_struct.h>
#include <event2/listener.h>

namespace kbms {

    class HttpContext {
    public:
        HttpContext(struct evhttp_request *req);

        virtual ~HttpContext();

        bool Initialize();

        std::string BodyByString() const;

        const char *OriUri() const;

        const std::string &Uri() const;

        void SetStatusCode(int code);

        int GetStatusCode() const;

        std::string GetRemoteIp() const;

        struct evhttp_request *GetReq() const;

        void AddResponseHeader(const std::string &key, const std::string &value);

        const char *FindRequestHeader(const char *key);

    private:
        //origin event request
        evhttp_request *req_;
        //request params after parser
        std::string uri_;
        std::string remote_ip_;

        const char *req_body_;
        size_t body_size_;

        int status_code_ = 200;

    };

    typedef std::shared_ptr<HttpContext> HttpContextPtr;

    typedef std::function<void(const std::string &response_data)> HTTPSendResponseCallback;

    typedef std::function<void(const HttpContextPtr &ctx, const HTTPSendResponseCallback &respcb)> HTTPRequestCallback;

    typedef std::map<std::string, HTTPRequestCallback> CallbackMap;

}
#endif // KBMS_SERVICE_HTTP_CONTEXT_H_ 
