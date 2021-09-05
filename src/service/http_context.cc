/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-07 03:31:50 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-02 06:19:44 PM
 @Modified By: dongzhixiong
************************************************************************/
#include "http_context.h"

namespace kbms {

    HttpContext::HttpContext(struct evhttp_request *req) : req_(req) {

    }

    HttpContext::~HttpContext() {

    }

    bool HttpContext::Initialize() {
        //evbuffer : the buffer of input and output
        struct evbuffer *evbuf = evhttp_request_get_input_buffer(req_);
        //buffer size
        body_size_ = evbuffer_get_length(evbuf);
        //buffer content
        if (body_size_ > 0) {
            req_body_ = (char *) evbuffer_pullup(evbuf, -1);
        }
        //url
        uri_ = evhttp_uri_get_path(req_->uri_elems);

        //remote_ip
        remote_ip_ = req_->remote_host;
        return true;

    }

    std::string HttpContext::BodyByString() const {
        return std::string(req_body_, body_size_);
    }

    const char *HttpContext::OriUri() const {
        return req_->uri;
    }

    const std::string &HttpContext::Uri() const {
        return uri_;
    }

    void HttpContext::SetStatusCode(int code) {
        status_code_ = code;
    }

    int HttpContext::GetStatusCode() const {
        return status_code_;
    }

    std::string HttpContext::GetRemoteIp() const {
        return remote_ip_;
    }

    void HttpContext::AddResponseHeader(const std::string &key, const std::string &value) {
        evhttp_add_header(req_->output_headers, key.data(), value.data());
    }

    const char *HttpContext::FindRequestHeader(const char *key) {
        return evhttp_find_header(req_->input_headers, key);
    }

    struct evhttp_request *HttpContext::GetReq() const {
        return req_;
    }


}
