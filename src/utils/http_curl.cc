/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-03-15 10:43:00 AM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 08:54:19 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <stdexcept>
#include <utility>
#include <cstring>

#include "curl.h"

#include "http_curl.h"
#include "helpers.h"
#include "logging.h"

class HttpCurl::Impl {

public:
    Impl() {
        curl_handle_ = curl_easy_init();
        if (!curl_handle_) {
            LOGGER_ERROR << "Couldn't initialize curl handle";
        }
        //default
        timeout_ = 50;
        follow_redirects_ = false;
        max_redirects_ = -1L;
        no_signal_ = false;
    }

    ~Impl() {
        this->Terminate();
    }

    //request
    void Post(const std::string &url, const std::string &data, Response &ret) {

        //specify request post
        curl_easy_setopt(GetCurlHandle(), CURLOPT_POST, 1L);

        //specify data
        curl_easy_setopt(GetCurlHandle(), CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(GetCurlHandle(), CURLOPT_POSTFIELDSIZE, data.size());

        //perform
        ExecuteRequest(url, &ret);
    }

    void Get(const std::string &url, Response &ret) {
        //perform
        ExecuteRequest(url, &ret);
    }

    void Del(const std::string &url, Response &ret) {
        const char *http_delete = "DELETE";
        curl_easy_setopt(GetCurlHandle(), CURLOPT_CUSTOMREQUEST, http_delete);
        //perform
        ExecuteRequest(url, &ret);
    }

    void Put(const std::string &url, const std::string &data, Response &ret) {

        // initialize data struct
        kbms::utils::PutDataStruct obj;
        obj.data = data.c_str();
        obj.length = data.size();

        // specify data
        curl_easy_setopt(GetCurlHandle(), CURLOPT_PUT, 1L);
        curl_easy_setopt(GetCurlHandle(), CURLOPT_UPLOAD, 1L);
        // specify callback function */
        curl_easy_setopt(GetCurlHandle(), CURLOPT_READFUNCTION, kbms::utils::ReadCallback);
        curl_easy_setopt(GetCurlHandle(), CURLOPT_READDATA, &obj);
        // must specify data size
        curl_easy_setopt(GetCurlHandle(), CURLOPT_INFILESIZE, static_cast<int64_t>(obj.length));

        //perform
        ExecuteRequest(url, &ret);
    }

    void SetTimeout(int timeout) {
        this->timeout_ = timeout;
    }

    void SetRedirects(bool follow_redirects, int max_redirects) {
        this->max_redirects_ = max_redirects;
        this->follow_redirects_ = follow_redirects;
    }

    void SetNoSignal(bool no_signal) {
        this->no_signal_ = no_signal;
    }

    void SetBasicAuth(const std::string &username, const std::string &password) {
        this->basicAuth.username = username;
        this->basicAuth.password = password;
    }

    void SetHeaders(HeaderFields &header_fields) {
        this->header_fields_ = header_fields;
    }

private:
    //stop
    void Terminate() {
        if (curl_handle_) {
            curl_easy_cleanup(curl_handle_);
        }
        curl_handle_ = nullptr;
    }

    //performance
    bool ExecuteRequest(const std::string &url, Response *ret) {
        //clear
        ret->body.clear();
        ret->code = 0;
        ret->headers.clear();

        // request headers
        struct curl_slist *headers = nullptr;

        // set http headers
        for (auto it = this->header_fields_.begin(); it != this->header_fields_.end(); ++it) {
            std::string header_string = it->first + ":" + it->second;
            headers = curl_slist_append(headers, header_string.c_str());
        }
        curl_easy_setopt(GetCurlHandle(), CURLOPT_HTTPHEADER, headers);

        //set url
        curl_easy_setopt(GetCurlHandle(), CURLOPT_URL, url.c_str());

        curl_easy_setopt(GetCurlHandle(), CURLOPT_WRITEFUNCTION, kbms::utils::WriteCallback);

        //set data object to pass to callback function
        curl_easy_setopt(GetCurlHandle(), CURLOPT_WRITEDATA, ret);
        //set the header callback function
        curl_easy_setopt(GetCurlHandle(), CURLOPT_HEADERFUNCTION, kbms::utils::HeaderCallback);
        //callback object for headers
        curl_easy_setopt(GetCurlHandle(), CURLOPT_HEADERDATA, ret);


        // set basic auth if configured
        if (this->basicAuth.username.length() > 0) {
            std::string auth_string = std::string(this->basicAuth.username + ":" + this->basicAuth.password);
            curl_easy_setopt(GetCurlHandle(), CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_easy_setopt(GetCurlHandle(), CURLOPT_USERPWD, auth_string.c_str());
        }
        // set error buffer
        curl_easy_setopt(GetCurlHandle(), CURLOPT_ERRORBUFFER, this->curl_errorbuf_);

        // set timeout
        if (this->timeout_) {
            curl_easy_setopt(GetCurlHandle(), CURLOPT_TIMEOUT_MS, this->timeout_);
            // dont want to get a sig alarm on timeout
            curl_easy_setopt(GetCurlHandle(), CURLOPT_NOSIGNAL, 1);
        }
        // set follow redirect
        if (this->follow_redirects_) {
            curl_easy_setopt(GetCurlHandle(), CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(GetCurlHandle(), CURLOPT_MAXREDIRS, static_cast<int64_t>(this->max_redirects_));
        }

        if (this->no_signal_) {
            // multi-threaded and prevent entering foreign signal handler
            curl_easy_setopt(GetCurlHandle(), CURLOPT_NOSIGNAL, 1);
        }


        CURLcode res;
        res = curl_easy_perform(GetCurlHandle());

        if (CURLE_OK != res) {
            int ret_code = res;
            if (ret_code > 99) ret_code = -1;
            ret->code = ret_code;
            ret->body = curl_easy_strerror(res);
        } else {
            int64_t code = 0;
            curl_easy_getinfo(GetCurlHandle(), CURLINFO_RESPONSE_CODE, &code);
            ret->code = static_cast<int>(code);
        }

        //free headerList
        curl_slist_free_all(headers);
        // reset curl handle
        curl_easy_reset(GetCurlHandle());
    }

    CURL *GetCurlHandle() {
        if (curl_handle_) {
            return curl_handle_;
        }
        throw std::runtime_error("Connection terminated");
    }

private:
    CURL *curl_handle_;

    //request params
    int timeout_;
    bool follow_redirects_;
    int max_redirects_;
    bool no_signal_;

    char curl_errorbuf_[CURL_ERROR_SIZE];
    struct {
        std::string username;
        std::string password;
    } basicAuth;

    HeaderFields header_fields_;
};

HttpCurl::HttpCurl() {
    impl_ = new Impl();
    if (!impl_) {
        LOGGER_ERROR << "Couldn't new Impl";
    }
}

HttpCurl::~HttpCurl() {
    if (impl_) {
        delete impl_;
    }
    impl_ = nullptr;
}

void HttpCurl::Post(const std::string &url, const std::string &request,
                    const HttpConnectInfo &info, Response &response) const {
    this->LoadInfo(info);
    const_cast<HttpCurl::Impl *>(impl_)->Post(url, request, response);
}

void HttpCurl::Put(const std::string &url, const std::string &request,
                   const HttpConnectInfo &info, Response &response) const {
    this->LoadInfo(info);
    const_cast<HttpCurl::Impl *>(impl_)->Put(url, request, response);
}

void HttpCurl::Get(const std::string &url,
                   const HttpConnectInfo &info, Response &response) const {
    this->LoadInfo(info);
    const_cast<HttpCurl::Impl *>(impl_)->Get(url, response);
}

void HttpCurl::Del(const std::string &url,
                   const HttpConnectInfo &info, Response &response) const {
    this->LoadInfo(info);
    const_cast<HttpCurl::Impl *>(impl_)->Del(url, response);
}

void HttpCurl::LoadInfo(const HttpConnectInfo &info) const {
    this->SetTimeout(info.timeout);
    this->SetBasicAuth(info.basicAuth.username, info.basicAuth.password);
    this->SetHeaders(info.headers);
}

void HttpCurl::SetTimeout(int timeout) const {
    const_cast<HttpCurl::Impl *>(impl_)->SetTimeout(timeout);
}

void HttpCurl::SetRedirects(bool follow_redirects, int max_redirects) const {
    const_cast<HttpCurl::Impl *>(impl_)->SetRedirects(follow_redirects, max_redirects);
}

void HttpCurl::SetNoSignal(bool no_signal) const {

    const_cast<HttpCurl::Impl *>(impl_)->SetNoSignal(no_signal);
}

void HttpCurl::SetBasicAuth(const std::string &username, const std::string &password) const {
    const_cast<HttpCurl::Impl *>(impl_)->SetBasicAuth(username, password);
}

void HttpCurl::SetHeaders(HeaderFields header_fields) const {
    const_cast<HttpCurl::Impl *>(impl_)->SetHeaders(header_fields);
}
