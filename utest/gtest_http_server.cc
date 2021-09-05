/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-24 08:15:43 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-05 08:34:25 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <iostream>
#include <sstream>
#include <random>
#include <unistd.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "service/http_server.h"
#include "base_gtest.h"
#include "utils/http_curl.h"

uint32_t port = 12138;
uint32_t thread_num = 5;

void DefaultHandler(const kbms::HttpContextPtr &ctx, const kbms::HTTPSendResponseCallback &cb) {

    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << "\n"
        << " ip=" << ctx->GetRemoteIp() << "\n"
        << " uri=" << ctx->Uri() << "\n"
        << " body=" << ctx->BodyByString() << "\n";

    cb(oss.str());
}


void RequestHandler(const kbms::HttpContextPtr &ctx, const kbms::HTTPSendResponseCallback &cb) {

    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << "\n"
        << "status:" << "OK" << "\n"
        << " ip=" << ctx->GetRemoteIp() << "\n"
        << " uri=" << ctx->Uri() << "\n"
        << " body=" << ctx->BodyByString() << "\n";

    static std::atomic<int> i(0);
    if (i++ == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    cb(oss.str());
}

void InitRequest() {
    HttpConnectInfo conn_info;
    std::shared_ptr<HttpCurl> http_curl = std::make_shared<HttpCurl>();
    Response response;
    std::string body = "begin to test server";
    std::string url = "http://127.0.0.1:12138/retry";
    http_curl->Post(url, body, conn_info, response);
}

void TestNormalRequest() {
    HttpConnectInfo conn_info;
    std::shared_ptr<HttpCurl> http_curl = std::make_shared<HttpCurl>();
    Response response;
    std::string body = "hello ke.com";
    std::string url = "http://127.0.0.1:12138/retry";
    http_curl->Post(url, body, conn_info, response);
    H_TEST_ASSERT(response.code == 200);
    H_TEST_ASSERT(!response.body.empty());
    H_TEST_ASSERT(response.body.find("uri=/retry") != std::string::npos);
    H_TEST_ASSERT(response.body.find("body=hello ke.com") != std::string::npos);
}

void TestDefaultRequest() {
    HttpConnectInfo conn_info;
    std::shared_ptr<HttpCurl> http_curl = std::make_shared<HttpCurl>();
    Response response;
    std::string body = "this default handler";
    std::string url = "http://127.0.0.1:12138/default";
    http_curl->Post(url, body, conn_info, response);
    H_TEST_ASSERT(response.code == 200);
    H_TEST_ASSERT(!response.body.empty());
    H_TEST_ASSERT(response.body.find("uri=/default") != std::string::npos);
    H_TEST_ASSERT(response.body.find("body=this default handler") != std::string::npos);
}

TEST_UNIT(gtestNormalHander) {
    bool r = kbms::HttpServer::Instance()->Init(port, thread_num);
    kbms::HttpServer::Instance()->RegisterDefaultFun(&DefaultHandler);
    kbms::HttpServer::Instance()->RegisterFun("/echo",&RequestHandler);
    std::shared_ptr<std::thread> t;
    t.reset(new std::thread(std::bind(&kbms::HttpServer::StartServer, kbms::HttpServer::Instance())));
    H_TEST_ASSERT(r);
    for (int i = 0;i < 4; ++i) {
        std::cout << "Running testHTTPClientRetry i=" <<i;

        InitRequest();
        TestNormalRequest();
    }
    usleep(1000 * 1000);
    pthread_cancel(t->native_handle());
    t->join();
}

TEST_UNIT(gtestDefaultHandler) {
    bool r = kbms::HttpServer::Instance()->Init(port, thread_num);
    kbms::HttpServer::Instance()->RegisterDefaultFun(&DefaultHandler);
    kbms::HttpServer::Instance()->RegisterFun("/retry",&RequestHandler);
    std::shared_ptr<std::thread> t;
    t.reset(new std::thread(std::bind(&kbms::HttpServer::StartServer, kbms::HttpServer::Instance())));
    H_TEST_ASSERT(r);
    for ( int i = 0; i < 4; ++i) {
        std::cout << "Running testHTTPClientRetry i=" << i;
        InitRequest();
        TestDefaultRequest();
    }
    usleep(1000 * 1000);
    pthread_cancel(t->native_handle());
    t->join();
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}
