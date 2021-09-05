ke-bear-microservice(kbms)
---

# 简介

[kbms]是一个基于[libevent]开发的C++11高性能网络框架，轻量且容易部署,目前主要支持HTTP协议。

# 特性

1. 非阻塞异步接口都是C++11特性设计的
1. 整个框架轻量,简洁
1. 功能单纯，专注网络
1. 上手简单，容易部署

# 项目历史

我们人工智能中心服务赋能组在实践过程中,对外提供功能主要以微服务方式,因此需要一套方便简洁的网络框架，在调研开源项目的过程中，没有发现一个合适的库来满足我们要求。结合我们自身的业务情况，理想中的C++网络框架应该满足以下几个特性：

1. 由于组内历史传统，最好是c++
2. 多线程，高性能网络服务器
2. 方便灵巧，很容易部署
2. 可以接入[spring-cloud-netflix](https://github.com/spring-cloud/spring-cloud-netflix "https://github.com/spring-cloud/spring-cloud-netflix")


# 快速入门

请见 [Quick Start](docs/quick_start.md)

# 示例

### An echo HTTP server

```cpp

#include "http_server.h"

int main(int argc, char* argv[]) {

    uint32_t port = 12138;
    uint32_t thread_num = 5;

    if(!kbms::HttpServer::Instance()->Init(port,thread_num)){
        std::cerr<<"initalize global resource failed."<<std::endl;
        return 0;
    }
    kbms::HttpServer::Instance()->RegisterDefaultFun(&DefaultHandler);
    kbms::HttpServer::Instance()->RegisterFun("/echo",[](const kbms::HttpContextPtr& ctx,const kbms::HTTPSendResponseCallback& cb)
                                                                { cb(ctx->BodyByString()); });
    kbms::HttpServer::Instance()->StartServer();
    while (true) {
        usleep(1);
    }

    return 0;
}

```
### 示例
更多示例可参考 [examples](examples/http)

# 感谢

1. 感谢[libevent], [glog](https://github.com/google/glog), [gtest](https://github.com/google/googletest), [jsoncpp]，[curl]，[wrk]等项目
1. [kbms]深度参考了[thrift]和[evpp]等项目的实现和设计，非常感谢[evpp](https://github.com/Qihoo360/evpp "https://github.com/Qihoo360/evpp") 和[thrift](https://github.com/apache/thrift "https://github.com/apache/thrift")


[gtest]:https://github.com/google/googletest
[libevent]:https://github.com/libevent/libevent
[Boost.Asio]:http://www.boost.org/
[evpp]:https://github.com/Qihoo360/evpp
[thrift]:https://github.com/apache/thrift
[jsoncpp]:https://github.com/open-source-parsers/jsoncpp
[curl]:https://github.com/curl
[wrk]:https://github.com/wg
