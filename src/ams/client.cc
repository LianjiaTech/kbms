/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-25 05:40:58 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:12:52 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <utility>
#include <functional>

#include "service/http_server.h"
#include "utils/logging.h"
#include "utils/http_curl.h"
#include "client.h"

namespace kbms {

    class Client::Impl {
    public:
        typedef std::function<void(const ReqBody &, ResBody &)> Func;
    private:
        std::map<std::string, Func> map_func;

    private:
        void RegistFunc(const std::string &key, Func pfunc) {
            map_func[key] = pfunc;
        }

        bool GenParams(const ReqBody &req_body,
                       std::string &url,
                       HttpConnectInfo &info) {

            std::string host;
            if (!kbms::HttpServer::Instance()->AcqInstance(req_body.service_name, host)) {
                LOGGER_ERROR << "fail to get instance:" << req_body.service_name;
                return false;
            }
            url = host + "/" + req_body.tail_affix;
            info.timeout = req_body.timeout;
            for (auto iter = req_body.req_headers.begin(); iter != req_body.req_headers.end(); iter++) {
                info.headers.insert(std::pair<std::string, std::string>(iter->first, iter->second));
            }

            if (!req_body.basicAuth.username.empty()
                && !req_body.basicAuth.password.empty()) {
                info.basicAuth.username = req_body.basicAuth.username;
                info.basicAuth.password = req_body.basicAuth.password;
            }
            return true;

        }

        void FormatRet(const std::string &url, const Response &response, ResBody &res_body) {
            res_body.size = response.body.size();
            res_body.res_buf = response.body;
            res_body.response = const_cast<char *>(res_body.res_buf.c_str());
            res_body.code = response.code;
            res_body.host = url;
        }

        void PostRequest(const ReqBody &req_body, ResBody &res_body) {
            Response response;
            std::string url;
            HttpConnectInfo info;
            if (!GenParams(req_body, url, info)) return;
            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();

            request->Post(url, req_body.request_body, info, response);

            FormatRet(url, response, res_body);
        }

        void GetRequest(const ReqBody &req_body, ResBody &res_body) {
            Response response;
            std::string url;
            HttpConnectInfo info;
            if (!GenParams(req_body, url, info)) return;
            std::shared_ptr<HttpCurl> request = std::make_shared<HttpCurl>();

            request->Get(url, info, response);
            FormatRet(url, response, res_body);
        }

    public:
        void Request(const std::string &type, const ReqBody &req_body, ResBody &res_body) {
            if (map_func.find(type) == map_func.end()) {
                LOGGER_ERROR << "fail to get this method" << type;
            } else {
                map_func[type](req_body, res_body);
            }
        }

        // default post
        void Request(const ReqBody &req_body, ResBody &res_body) {
            Request("Post", req_body, res_body);
        }

        Impl() {
            // regist post function
            auto f_post = std::bind(&Impl::PostRequest, this, std::placeholders::_1, std::placeholders::_2);
            RegistFunc("Post", f_post);
            // regist get function
            auto f_get = std::bind(&Impl::GetRequest, this, std::placeholders::_1, std::placeholders::_2);
            RegistFunc("Get", f_get);
        }

        ~Impl() {

        }
    };

    Client::Client() : impl_(new Impl()) {

    }

    Client::~Client() {
        if (impl_) delete impl_;
        impl_ = nullptr;
    }

    void Client::Request(const std::string &type, const ReqBody &req_body, ResBody &res_body) const {
        const_cast<Client::Impl *>(impl_)->Request(type, req_body, res_body);
    }

    void Client::Request(const ReqBody &req_body, ResBody &res_body) const {
        const_cast<Client::Impl *>(impl_)->Request(req_body, res_body);
    }

}
