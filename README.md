ke-bear-microservice(kbms)
---

# Introduction [中文说明](README_cn.md)

KBMS is a C++11 high performance network framework based on libevent. It is light and easy to deploy. At present, it mainly supports HTTP protocol. 

# Features

1. Non-blocking pure asynchronous multithreaded HTTP server
1. Light and concise
1. Simple function, focus on Network
1. Simple and easy to deploy

# Origin
In the process of practice, the service enabling group of our AI center mainly provides external functions in the form of micro services, so we need a convenient and concise network framework. In the process of investigating open source projects, we did not find a suitable library to meet our requirements. According to our own business situation, the ideal C + + network framework should meet the following characteristics:

1. Due to the historical tradition of the group, the best is C++
2. Multi thread, high performance network server
2. Handy and easy to deploy
2. Can  access to [spring-cloud-netflix](https://github.com/spring-cloud/spring-cloud-netflix "https://github.com/spring-cloud/spring-cloud-netflix")

# Getting Started
Please see [Quick Start](docs/quick_start.md)
# Examples

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

### More examples
Please see the source code in [examples](examples/http)
# Thanks

1. Thanks for [libevent], [glog](https://github.com/google/glog), [gtest](https://github.com/google/googletest), [jsoncpp]，[curl]，[wrk] projects
1. [kbms] is highly inspired by [thrift] and [evpp],Thanks [evpp](https://github.com/Qihoo360/evpp "https://github.com/Qihoo360/evpp") and [thrift](https://github.com/apache/thrift "https://github.com/apache/thrift")


[gtest]:https://github.com/google/googletest
[libevent]:https://github.com/libevent/libevent
[Boost.Asio]:http://www.boost.org/
[evpp]:https://github.com/Qihoo360/evpp
[thrift]:https://github.com/apache/thrift
[jsoncpp]:https://github.com/open-source-parsers/jsoncpp
[curl]:https://github.com/curl
[wrk]:https://github.com/wg
