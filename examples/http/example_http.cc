/*************************************************************************
 @Copyright (c), ke Inc
 @Created Time : 2021-02-24 08:15:43 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 05:26:02 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <iostream>
#include <random>
#include <unistd.h>
#include <cmath>


#include "service/http_server.h"

void DefaultHandler(const kbms::HttpContextPtr &ctx, const kbms::HTTPSendResponseCallback &cb) {
    cb(std::string("this default handler"));
}

void ExpHandler(const kbms::HttpContextPtr &ctx, const kbms::HTTPSendResponseCallback &cb) {
    cb(std::string("operation is timeout"));
}

void Func(const kbms::HttpContextPtr &ctx, const kbms::HTTPSendResponseCallback &cb) {
    std::string request_body = ctx->BodyByString();
    Json::Value result;
    result["code"] = 1;
    result["msg"] = "success";
    result["query"] = request_body;
    result["data"] = request_body.size();
    cb(result.toStyledString());

}

int main() {

    uint32_t port = 12138;
    uint32_t thread_num = 1;

    if (!kbms::HttpServer::Instance()->Init(port, thread_num)) {
        std::cerr << "initalize global resource failed." << std::endl;
        return 0;
    }
    kbms::HttpServer::Instance()->RegisterDefaultFun(&DefaultHandler);
    kbms::HttpServer::Instance()->RegisterExpireFun(&ExpHandler);
    kbms::HttpServer::Instance()->RegisterFun("/echo", &Func);
    kbms::HttpServer::Instance()->StartServer();
    while (true) {
        usleep(1);
    }
    return 0;
}
