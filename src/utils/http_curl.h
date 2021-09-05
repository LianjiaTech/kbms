/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-03-12 11:35:00 AM
 @Created By : dongzhixiong
 @Description: encapsulation curl
 @Last modified: 2021-08-03 12:09:41 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_UTILS_HTTP_CURL_H_
#define KBMS_UTILS_HTTP_CURL_H_

#include <string>
#include <iostream>
#include <map>

typedef std::map<std::string, std::string> HeaderFields;

// request params
struct HttpConnectInfo {
    //set header
    HeaderFields headers;
    //ms
    int timeout;
    struct {
        //user name
        std::string username;
        //passward
        std::string password;
    } basicAuth;

    HttpConnectInfo() : timeout(50) {}
};

//response
struct Response {
    int code;
    std::string body;
    HeaderFields headers;
};

class HttpCurl {
public:
    HttpCurl();

    virtual ~HttpCurl();

    void Post(const std::string &url, const std::string &request,
              const HttpConnectInfo &info, Response &response) const;

    void Put(const std::string &url, const std::string &request,
             const HttpConnectInfo &info, Response &response) const;

    void Get(const std::string &url,
             const HttpConnectInfo &info, Response &response) const;

    void Del(const std::string &url,
             const HttpConnectInfo &info, Response &response) const;

private:
    void SetTimeout(int timeout) const;

    void SetRedirects(bool follow_redirects, int max_redirects) const;

    void SetNoSignal(bool no_signal) const;

    void SetBasicAuth(const std::string &username, const std::string &password) const;

    void SetHeaders(HeaderFields header_fields) const;

    void LoadInfo(const HttpConnectInfo &info) const;

private:
    class Impl;

    Impl *impl_;
};

#endif  // KBMS_UTILS_HTTP_CURL_H_
