/*************************************************************************
 @Copyright (c), ke Inc
 @Created Time : 2021-02-24 08:15:43 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-05 02:25:57 PM
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

int main() {

    uint32_t port = 12138;
    uint32_t thread_num = 2;

    if (!kbms::HttpServer::Instance()->Init(port, thread_num)) {
        std::cerr << "initalize global resource failed." << std::endl;
        return 0;
    }
    kbms::HttpServer::Instance()->RegisterDefaultFun(&DefaultHandler);
    kbms::HttpServer::Instance()->RegisterFun("/echo", [](const kbms::HttpContextPtr &ctx,
                                                          const kbms::HTTPSendResponseCallback &cb) {
        cb(ctx->BodyByString());
    });
    kbms::HttpServer::Instance()->StartServer();
    while (true) {
        usleep(1);
    }
    return 0;
}
